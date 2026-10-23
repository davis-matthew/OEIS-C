/*
 * A024882  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = A023531, t = (Lucas numbers).
 * https://oeis.org/A024882
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊n/2⌋} s(i)·L_{n-i+1}, where s(i)=1 if i+1 is a
 *            triangular number (>1), otherwise 0, and L_m is the Lucas
 *            sequence (L_0=2, L_1=1, L_m=L_{m-1}+L_{m-2})
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t lucas_max = 0;      /* number of cached values (>=2 once allocated) */
static seq_t *lucas_cache = NULL;

/* Extend lucas_cache to include values up to index n (inclusive). */
static void ensure_lucas(idx_t n)
{
    if (lucas_max > 0 && lucas_max - 1 >= n) return; /* already enough */

    idx_t new_max = n + 1;                         /* indices 0 … n */
    seq_t *tmp = (seq_t *)realloc(lucas_cache,
                                  new_max * sizeof(seq_t));
    if (!tmp) abort();           /* allocation failed */
    lucas_cache = tmp;

    if (lucas_max == 0) {                     /* first time */
        lucas_cache[0] = (seq_t)2;
        lucas_cache[1] = (seq_t)1;
        lucas_max = 2;
    } else {                                   /* extend */
        for (idx_t i = lucas_max; i < new_max; ++i)
            lucas_cache[i] = lucas_cache[i-1] + lucas_cache[i-2];
        lucas_max = new_max;
    }
}

/* Return a(n) for any n ≥ 2.  For n < 2 the value is unspecified. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;

    ensure_lucas(n);

    idx_t k   = n / 2;                        /* floor(n/2) */
    seq_t sum = (seq_t)0;

    /* s(i)=1 when i+1 is a triangular number.  Triangular numbers: T_m = m(m+1)/2. */
    for (idx_t m = 2;; ++m) {
        __int128 t = (__int128)m * (m + 1) / 2;   /* T_m */
        if (t > ( __int128)k + 1) break;          /* need only i ≤ k */

        idx_t i = (idx_t)(t - 1);                 /* index where s(i)=1 */
        if (i > k) break;

        sum += lucas_cache[n - i + 1];            /* t(n-i+1) = Lucas index n-i+1 */
    }

    return sum;
}
