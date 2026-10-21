/*
 * A083071  First subdiagonal of number array A083064.
 * https://oeis.org/A083071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n+1)*(n+3)^n + 1)/(n+2) (OEIS A083071)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For 64‑bit builds use 128‑bit arithmetic to avoid overflow of the power.
       For larger seq_t (e.g. 128‑bit or arbitrary precision) rely on seq_t
       operations which themselves handle large integers. */
    if (sizeof(seq_t) == 8) {
        unsigned __int128 base = (unsigned __int128)(n + 3);
        unsigned __int128 power = 1;
        for (idx_t i = 0; i < n; ++i) {
            power *= base;
        }
        unsigned __int128 num = (unsigned __int128)(n + 1) * power + 1;
        unsigned __int128 res = num / (n + 2);
        return (seq_t)res;
    } else {
        seq_t base = (seq_t)(n + 3);
        seq_t power = 1;
        for (idx_t i = 0; i < n; ++i) {
            power *= base;
        }
        seq_t numerator = power * (seq_t)(n + 1) + 1;
        return numerator / (seq_t)(n + 2);
    }
}
