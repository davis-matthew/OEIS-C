/*
 * A056976  Number of blocks of {0, 1, 0} in the binary expansion of n.
 * https://oeis.org/A056976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = a(n) + [n>1 and n ≡ 1 (mod 4)], a(2n+1) = a(n), a(1) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return 0;
    }
    if ((n & 1ULL) == 0) {
        idx_t m = n >> 1;
        seq_t res = a(m);
        if (m > 1 && (m & 3ULL) == 1ULL) {
            res += (seq_t)1;
        }
        return res;
    } else {
        return a(n >> 1);
    }
}
