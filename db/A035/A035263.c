/*
 * A035263  Trajectory of 1 under the morphism 0 -> 11, 1 -> 10; parity of
 *          2-adic valuation of 2n: a(n) = A000035(A001511(n)).
 * https://oeis.org/A035263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   a(2*n) = 1 - a(n); a(2*n+1) = 1 (for all n ≥ 1)
 * Range:     u8 n <= 65536, u16 n <= 65536, u32 n <= 65536, u64 n <= 65536,
 *            u128 n <= 65536, big any n
 * Validated: standard, pass, 121 terms through n = 9690 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;          /* will hold the value of a(n). */
    while ((n & 1) == 0)   /* while n is even */
    {
        n >>= 1;           /* equivalent to n = n / 2. */
        res = 1 - res;     /* apply the recurrence a(2*m) = 1 - a(m). */
    }
    /* When n becomes odd, a(n) = 1 by definition.  `res` already represents this. */
    return res;
}
