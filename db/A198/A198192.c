/*
 * A198192  Replace 2^k in the binary representation of n with n-k (i.e. if n
 *          = 2^a + 2^b + 2^c + ... then a(n) = (n-a) + (n-b) + (n-c) + ...).
 * https://oeis.org/A198192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = sum_{k: bit k set} (n - k) (equivalent to n*A000120(n) -
 *            A073642(n))
 * Range:     u8 n <= 54, u16 n <= 6014, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t sn = (seq_t)n;
    idx_t pos = 0;
    idx_t temp = n;
    while (temp) {
        if (temp & 1) {
            result += sn;
            result -= pos;
        }
        temp >>= 1;
        ++pos;
    }
    return result;
}
