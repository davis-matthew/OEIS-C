/*
 * A203155  (n-1)-st elementary symmetric function of {3, 3, 4, 4, 5, 5,...,
 *          Floor[(n+5)/2]}.
 * https://oeis.org/A203155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = e_{n-1}({3,3,4,4,5,5,..., floor[(n+5)/2]}) where e_k
 *            denotes the k-th elementary symmetric function of the multiset
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* compute the product P = ∏_{i=1}^{n} a_i,
       where a_i = ceil(i/2) + 2  (the i-th element of the multiset)   */
    seq_t P = 1;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t pair = (i + 1) / 2;          /* ceil(i/2) */
        seq_t val = (seq_t)pair + 2;       /* a_i */
        P *= val;
    }

    /* e_{n-1} = Σ_{i=1}^{n} P / a_i  */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t pair = (i + 1) / 2;
        seq_t val = (seq_t)pair + 2;
        sum += P / val;                    /* exact division */
    }
    return sum;
}
