/*
 * A129476  a(n) is the concatenation in increasing order of all single-digit
 *          divisors of n.
 * https://oeis.org/A129476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenation of the digits d∈{1,…,9} that divide n, in
 *            increasing order
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 5040, u64 n <= 5040,
 *            u128 n <= 5040, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (int d = 1; d <= 9; ++d) {
        if (n % d == 0) {
            res = res * 10 + d;
        }
    }
    return res;
}
