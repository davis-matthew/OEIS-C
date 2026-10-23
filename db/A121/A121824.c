/*
 * A121824  Primes of the form (3^n + 5^n)/2.
 * https://oeis.org/A121824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, nonn, bref
 * Formula:   a(n) = (3^(2^n) + 5^(2^n)) / 2, n >= 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t *pow3 = NULL;
static seq_t *pow5 = NULL;
static size_t    cap  = 0;

/* Ensure the power arrays have space for indices up to n.
   Also compute any powers not yet stored.  */
static void
ensure_cap(idx_t n)
{
    if ((size_t)n <= cap)
        return;
    /* grow to at least n+1, keep power1 at index 1 for n=1 */
    size_t new_cap = n + 1;
    pow3 = (seq_t *)realloc(pow3, new_cap * sizeof(seq_t));
    pow5 = (seq_t *)realloc(pow5, new_cap * sizeof(seq_t));
    if (cap == 0) {
        /* initialise for n=1 (exponent 2) */
        pow3[1] = (seq_t)9;   /* 3^2 */
        pow5[1] = (seq_t)25;  /* 5^2 */
        cap = 1;
    }
    /* compute any missing powers */
    for (size_t i = cap + 1; i <= (size_t)n; ++i) {
        pow3[i] = pow3[i-1] * pow3[i-1];
        pow5[i] = pow5[i-1] * pow5[i-1];
    }
    cap = (size_t)n;
}

seq_t
a(idx_t n)
{
    if (n == 0)   /* not defined – return 0 */
        return (seq_t)0;
    ensure_cap(n);
    return (pow3[n] + pow5[n]) / (seq_t)2;
}
