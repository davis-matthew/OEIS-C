/*
 * A004280  2 together with the odd numbers (essentially the result of the
 *          first stage of the sieve of Eratosthenes).
 * https://oeis.org/A004280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n - 3 + floor(2/n)
 * Range:     u8 n <= 129, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return 2 * sn - 3 + 2 / sn;
}
