/*
 * A215457  Position of 2n-1 in A215456.
 * https://oeis.org/A215457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with (4k-1)!! mod 2k = 2n-1 (A215456(k) = 2n-1);
 *            found by search starting at k = n
 * Range:     u8 n <= 33, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 79 terms through n = 918 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t target = (seq_t)2 * (seq_t)n - 1;
    for (idx_t k = n;; k++) {
        seq_t mod = (seq_t)2 * (seq_t)k;
        seq_t lim = (seq_t)4 * (seq_t)k - 1;
        seq_t res = 1 % mod;
        for (seq_t i = 1; i <= lim; i += 2) {
            res = (res * i) % mod;
            if (res == 0)
                break;
        }
        if (res == target)
            return (seq_t)k;
    }
}
