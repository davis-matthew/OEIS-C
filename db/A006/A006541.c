/*
 * A006541  Number of dissimilarity relations on an n-set.
 * https://oeis.org/A006541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{i=0..m} (m-i)!*Stirling2(m, m-i), where m =
 *            n*(n-1)/2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    seq_t m_seq = ((seq_t)n * ((seq_t)n - 1)) / 2;
    idx_t m = (idx_t)m_seq;
    
    seq_t **S = (seq_t **)malloc((m + 1) * sizeof(seq_t *));
    for (idx_t i = 0; i <= m; i++) {
        S[i] = (seq_t *)calloc(i + 1, sizeof(seq_t));
    }
    
    S[0][0] = 1;
    
    for (idx_t n_idx = 1; n_idx <= m; n_idx++) {
        for (idx_t k = 1; k <= n_idx; k++) {
            seq_t term1 = (k <= n_idx - 1) ? ((seq_t)k * S[n_idx - 1][k]) : 0;
            seq_t term2 = S[n_idx - 1][k - 1];
            S[n_idx][k] = term1 + term2;
        }
    }
    
    seq_t *fact = (seq_t *)malloc((m + 1) * sizeof(seq_t));
    fact[0] = 1;
    for (idx_t i = 1; i <= m; i++) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }
    
    seq_t result = 0;
    for (idx_t i = 0; i <= m; i++) {
        idx_t j = m - i;
        result = result + fact[j] * S[m][j];
    }
    
    for (idx_t i = 0; i <= m; i++) {
        free(S[i]);
    }
    free(S);
    free(fact);
    
    return result;
}
