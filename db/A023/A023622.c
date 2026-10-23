/*
 * A023622  Convolution of Lucas numbers and A001950.
 * https://oeis.org/A023622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{n} L_k * B_{n+1-k} where L_k are Lucas numbers
 *            (L_1 = 1, L_2 = 3, L_{k} = L_{k-1} + L_{k-2}) and B_m = m + ⌊m *
 *            φ⌋ with φ = (1+√5)/2 ≈ 1.61803398875
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* the sequence is defined for n ≥ 1 */

    /* constant golden ratio */
    static const double PHI = 1.6180339887498948482045868343656381177203091798058;

    seq_t sum = 0;

    /* initial Lucas numbers */
    seq_t l_prev = 1;   /* L_1 */
    seq_t l_curr = 3;   /* L_2 */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t Li;              /* current Lucas number L_i */
        if (i == 1)
            Li = l_prev;        /* 1 */
        else if (i == 2)
            Li = l_curr;        /* 3 */
        else {
            Li = l_curr + l_prev;
            l_prev = l_curr;
            l_curr = Li;
        }

        /* compute B_{n+1-i} = (n+1-i) + floor((n+1-i) * φ) */
        idx_t k = n + 1 - i;
        /* floor((double)k * PHI) is obtained by truncation to uint64_t */
        uint64_t floor_part = (uint64_t)((double)k * PHI);
        seq_t  bk = (seq_t)k + (seq_t)floor_part;

        sum += Li * bk;
    }

    return sum;
}
