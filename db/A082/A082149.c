/*
 * A082149  A transform of C(n,2).
 * https://oeis.org/A082149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n,2)*(3^(n-2)+1)/2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = n*(n-1)/2 * (3^(n-2)+1)/2 */

#include <stdbool.h>   /* for completeness, not used directly */

static seq_t pow3(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 3;
    while (exp > 0)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* C(n,2) = 0 for n = 0 or 1 */

    seq_t binom = (seq_t)n * ((seq_t)n - (seq_t)1) / (seq_t)2;
    seq_t pw3   = pow3(n - 2);    /* 3^(n-2) */
    seq_t half = (pw3 + (seq_t)1) / (seq_t)2;  /* (3^(n-2)+1)/2 is always integer */
    return binom * half;
}
