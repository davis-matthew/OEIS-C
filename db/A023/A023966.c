/*
 * A023966  First digit after decimal point of 7th root of n.
 * https://oeis.org/A023966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(10 * (n^(1/7) - floor(n^(1/7))))
 * Range:     u8 n <= 1, u16 none, u32 n <= 396, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;

    /* Compute the integer part of the seventh root of n */
    seq_t k = 0;
    for (;;)
    {
        seq_t kp1 = k + 1;
        seq_t p = 1;
        for (int i = 0; i < 7; ++i) p *= kp1;  /* kp1^7 */
        if (p > n_seq) break;
        k = kp1;
    }

    /* Precompute 10^7 */
    seq_t ten_pow7 = (seq_t)10;
    for (int i = 1; i < 7; ++i) ten_pow7 *= 10;  /* 10^7 = 10,000,000 */

    seq_t n_times10pow = n_seq * ten_pow7;

    seq_t k10 = k * 10;

    seq_t ans = 0;
    for (int d = 0; d <= 9; ++d)
    {
        seq_t base = k10 + d;
        seq_t base_pow = 1;
        for (int i = 0; i < 7; ++i) base_pow *= base;  /* base^7 */
        if (base_pow > n_times10pow) break;
        ans = d;
    }

    return ans;
}
