/*
 * A100061  Numerator of the probability that an integer n occurs in the
 *          cumulative sums of the decimal digits of a random real number
 *          between 0 and 1.
 * https://oeis.org/A100061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A100061 – numerator of the probability that an integer n occurs
   in the cumulative sums of the decimal digits of a random real number
   between 0 and 1.
   Recurrence (order 9):
     a(1) = 1
     a(2) = 10
     a(3) = 100
     a(4) = 1 000
     a(5) = 10 000
     a(6) = 100 000
     a(7) = 1 000 000
     a(8) = 10 000 000
     a(9) = 100 000 000
     a(n) = a(n‑1)+9 a(n‑2)+81 a(n‑3)+729 a(n‑4)+6561 a(n‑5)+59049 a(n‑6)
            +531441 a(n‑7)+4782969 a(n‑8)+43046721 a(n‑9)   for n ≥ 10 */

#include <stdlib.h>

/* recurrence coefficients for a(n)=a(n‑1)+9 a(n‑2)+ … +43046721 a(n‑9) */
static const seq_t COEFF[9] = {
    (seq_t)1, (seq_t)9, (seq_t)81, (seq_t)729, (seq_t)6561,
    (seq_t)59049, (seq_t)531441, (seq_t)4782969, (seq_t)43046721
};

/* initial 9 terms of the sequence (1‑indexed) */
static const seq_t INIT[10] = {
    (seq_t)0,      /* ignore index 0 */
    (seq_t)1,      /* a(1)  */
    (seq_t)10,     /* a(2)  */
    (seq_t)100,    /* a(3)  */
    (seq_t)1000,   /* a(4)  */
    (seq_t)10000,  /* a(5)  */
    (seq_t)100000, /* a(6)  */
    (seq_t)1000000,/* a(7)  */
    (seq_t)10000000,/* a(8) */
    (seq_t)100000000 /* a(9) */
};

/* cache of already computed values, 1‑indexed */
static seq_t *CACHE = NULL;
static idx_t  CACHE_SIZE = 0;   /* allocated array length (≥ CACHE_MAX+1) */
static idx_t  CACHE_MAX = 0;    /* largest n that has been computed */

/* Ensure that CACHE[n] is valid (create or extend buffer if necessary) */
static void ensure_cache(idx_t n)
{
    if (n <= CACHE_MAX) return;                 /* already available */

    /* grow buffer to fit indices up to n (1‑based), allocate at least n+1 */
    idx_t new_size = n + 1;
    if (new_size < CACHE_SIZE) new_size = CACHE_SIZE;

    seq_t *tmp = (seq_t *)realloc(CACHE, new_size * sizeof(seq_t));
    if (!tmp) exit(1);                /* allocation failed */
    CACHE = tmp;
    CACHE_SIZE = new_size;

    /* compute terms from the current maximum up to n */
    for (idx_t i = CACHE_MAX + 1; i <= n; ++i) {
        if (i <= 9) {
            CACHE[i] = INIT[i];
        } else {
            seq_t sum = 0;
            for (int k = 1; k <= 9; ++k) {
                sum += CACHE[i - k] * COEFF[k - 1];
            }
            CACHE[i] = sum;
        }
    }
    CACHE_MAX = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset 1 – not used */
    ensure_cache(n);
    return CACHE[n];
}
