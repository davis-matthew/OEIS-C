/*
 * A070710  n^7 mod 29.
 * https://oeis.org/A070710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 29
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t mod = 29;
    seq_t res = 1;
    seq_t base = (seq_t)n % mod;

    idx_t e = 7;
    while (e) {
        if (e & 1U) res = (res * base) % mod;
        base = (base * base) % mod;
        e >>= 1;
    }

    return res;
}
