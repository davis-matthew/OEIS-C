/*
 * A103726  Column m=9 sequence of triangle A103718(n,m), n>=0, without
 *          leading zeros.
 * https://oeis.org/A103726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |S1(n+10,10)| + |S1(n+10,11)| (unsigned Stirling numbers
 *            of the first kind)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Stirling numbers of the first kind up to row n+10, k=0..11 */
    static seq_t **S = NULL;          /* 2D table: S[row][k] */
    static size_t max_row = 0;        /* highest row computed so far */
    const size_t K = 12;              /* columns: 0..11 */

    const size_t target_row = (size_t)(n + 10); /* inclusive row needed */

    /* Initialise table on first use */
    if (S == NULL) {
        S = (seq_t**)malloc(sizeof(seq_t*) * (target_row + 1));
        S[0] = (seq_t*)calloc(K, sizeof(seq_t));
        S[0][0] = 1; /* S(0,0) = 1 */
        max_row = 0;
    }

    /* Allocate more rows if necessary */
    if (target_row > max_row) {
        const size_t new_size = target_row + 1;
        S = (seq_t**)realloc(S, sizeof(seq_t*) * new_size);
        for (size_t r = max_row + 1; r <= target_row; ++r) {
            S[r] = (seq_t*)calloc(K, sizeof(seq_t));
            if (r > 0) {
                /* S(r,0) = 0 already from calloc */
                const size_t k_end = (r < 11) ? r : 11;
                for (size_t k = 1; k <= k_end; ++k) {
                    seq_t term1 = S[r - 1][k - 1];
                    seq_t term2 = (seq_t)(r - 1) * S[r - 1][k];
                    S[r][k] = term1 + term2;
                }
            }
        }
        max_row = target_row;
    }

    /* Retrieve required Stirling numbers */
    seq_t s10 = S[(size_t)(n + 10)][10];
    seq_t s11 = S[(size_t)(n + 10)][11]; /* zero if n+10 < 11 */

    return s10 + s11;
}
