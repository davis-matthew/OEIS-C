/*
 * A133025  Divisors of 33550336, the 5th perfect number.
 * https://oeis.org/A133025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy
 * Formula:   a(n) = 2^(n-1) for 1 ≤ n ≤ 13 a(n) = 2^(n-1) - 2^(n-14) for 14 ≤
 *            n ≤ 26
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n > 26 the sequence has no further terms, return 0. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1, but guard for safety */
    if (n <= 13) {
        /* first 13 powers of 2 */
        return ((seq_t)1 << (n - 1));
    } else if (n <= 26) {
        /* remaining 13 divisors: 8191 * 2^(n-14) = 2^(n-1) - 2^(n-14) */
        return ((seq_t)1 << (n - 1)) - ((seq_t)1 << (n - 14));
    } else {
        return 0;   /* sequence ends at n = 26 */
    }
}
