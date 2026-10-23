/*
 * A027790  a(n) = 10*(n+1)*binomial(n+3,5)/3.
 * https://oeis.org/A027790
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+1,3) * binomial(n+3,3)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 72, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute C(n,3) for n >= 3 using a multiplicative formula.
   It avoids overflow for all n that fit the 64‑bit binomial range
   and works with arbitrary‑precision seq_t. */
static seq_t binom3(idx_t n)
{
    seq_t r = 1;
    r = r * ((seq_t)n - 2) / 1;   /* multiply by (n-2) */
    r = r * ((seq_t)n - 1) / 2;   /* multiply by (n-1) and divide by 2 */
    r = r * (seq_t)n / 3;         /* multiply by n and divide by 3 */
    return r;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;   /* value undefined for n < 2, return 0 */

    /* C(n+1,3) */
    seq_t c1 = binom3(n + 1);

    /* C(n+3,3) */
    seq_t c2 = binom3(n + 3);

    return c1 * c2;
}
