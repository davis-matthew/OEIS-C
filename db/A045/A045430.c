/*
 * A045430  Number of dominoes with n spots (in standard set).
 * https://oeis.org/A045430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   a(n) = |{(i,j) : i,j ∈ {0,…,6}, i + j = n, i ≤ j}|
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 12) return 0;
    seq_t nn = (seq_t)n;
    seq_t start = 0;
    if (nn > 6) start = nn - 6;
    seq_t end = nn / 2;
    if (start > end) return 0;
    return end - start + 1;
}
