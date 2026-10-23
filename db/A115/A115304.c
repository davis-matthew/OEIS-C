/*
 * A115304  a(n) = n if n < 4, otherwise 4*a(floor(n/4)) + 3 - n mod 4.
 * https://oeis.org/A115304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n if n < 4 else 4*a(floor(n/4)) + 3 - n mod 4
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4u) return (seq_t)n;          /* Base cases: a(1)=1, a(2)=2, a(3)=3 */
    idx_t q = n >> 2;                    /* floor(n/4) */
    seq_t sub = a(q);                    /* Recursive subproblem */
    return sub * 4 + 3 - (seq_t)(n & 3); /* 3 - (n mod 4) is always non‑negative */
}
