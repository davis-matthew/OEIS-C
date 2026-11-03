/*
 * A053296  Partial sums of A053295.
 * https://oeis.org/A053296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 32, u64 n <= 78, u128 n <= 171,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 752 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute C(x,6) with a multiplicative formula.  x must be ≥ 6. */
static inline seq_t binom_n6(idx_t x)
{
    seq_t r = (seq_t)1;
    /* r *= x * (x-1) * (x-2) * (x-3) * (x-4) * (x-5) */
    for (idx_t i = 0; i < 6; ++i)
        r *= (seq_t)(x - i);
    r /= (seq_t)720;                /* 6! = 720 */
    return r;
}

/* Recurrence: a(0)=1, a(1)=8,
   a(n) = a(n-1) + a(n-2) + C(n+6,6)   (n ≥ 2)   */

/* Sequence a(n) = partial sums of A053295.  */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    seq_t a_nm2 = (seq_t)1;          /* a(n-2)  */
    seq_t a_nm1 = (seq_t)8;          /* a(n-1)  */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t c = binom_n6(i + 6);   /* C(i+6,6) */
        seq_t a_n  = a_nm1 + a_nm2 + c;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
