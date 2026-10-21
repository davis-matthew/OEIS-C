/*
 * A050487  Geometric Connell sequence: start with 1; then next two numbers ==
 *          2 mod 3; next four == 3 mod 3; next eight == 1 mod 3; etc.
 * https://oeis.org/A050487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*n - 2*(1 + floor(log_2(n)))
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t floor_log2_idx(idx_t n)
{
    /* assumes n >= 1 */
    idx_t r = 0;
    while (n >>= 1)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* compute a(n) for n >= 1 */
    idx_t lg = floor_log2_idx(n);
    seq_t res = (seq_t)3 * (seq_t)n - (seq_t)2 * ((seq_t)1 + (seq_t)lg);
    return res;
}
