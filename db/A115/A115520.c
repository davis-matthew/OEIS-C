/*
 * A115520  Number of ordered triples (i,j,k) in range [0..n] satisfying i ==
 *          j mod 2 and j == k mod 3.
 * https://oeis.org/A115520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (i,j,k) in [0..n] with i ≡ j
 *            (mod 2) and j ≡ k (mod 3). Computation is by counting residues
 *            modulo 6
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* counts of i for each parity */
    seq_t even_i = (seq_t)(n / 2) + 1;
    seq_t odd_i  = (((seq_t)n) + 1) - even_i;

    /* counts of k for each residue mod 3 */
    seq_t base_k = (seq_t)(n / 3);
    idx_t  rem_k = n % 3;
    seq_t k_cnt[3] = {
        base_k + 1,
        (rem_k >= 1) ? base_k + 1 : base_k,
        (rem_k >= 2) ? base_k + 1 : base_k
    };

    /* counts of j for each residue mod 6 */
    seq_t base_j = (seq_t)(n / 6);
    idx_t rem_j  = n % 6;

    seq_t res = 0;
    for (idx_t r = 0; r < 6; ++r) {
        seq_t cnt_j = base_j + ((r <= rem_j) ? 1 : 0);
        seq_t cnt_i = ((r & 1) == 0) ? even_i : odd_i;
        seq_t cnt_k = k_cnt[r % 3];
        res += cnt_j * cnt_i * cnt_k;
    }
    return res;
}
