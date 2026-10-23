/*
 * A015085  Carlitz-Riordan q-Catalan numbers (recurrence version) for q=4.
 * https://oeis.org/A015085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = Σ_{i=0..n} 4^i · a(i) · a(n-i) , a(0) = 1 Implements
 *            on‑demand memoisation with O(n²) time per new value
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static caches for the sequence values and powers of 4   */
    static seq_t *seq = NULL;     /* seq[k] == a(k)  */
    static idx_t seq_cap = 0;     /* number of entries stored   */
    static seq_t *pwr = NULL;     /* pwr[i] == 4^i   */
    static idx_t pwr_cap = 0;     /* highest power stored     */

    /* initialise a(0) and 4^0 the first time */
    if (seq_cap == 0) {
        seq = (seq_t *)malloc(sizeof(seq_t));
        if (!seq) return 0;           /* out of memory – caller will handle */
        seq[0] = (seq_t)1;
        seq_cap = 1;

        pwr = (seq_t *)malloc(sizeof(seq_t));
        if (!pwr) return 0;
        pwr[0] = (seq_t)1;
        pwr_cap = 1;
    }

    /* growth of the sequence cache to at least n+1 entries   */
    if (seq_cap <= n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        seq = tmp;
        /* initialise new slots to 0 */
        for (idx_t i = seq_cap; i < (idx_t)new_cap; ++i) seq[i] = 0;
        idx_t old_cap = seq_cap;
        seq_cap = (idx_t)new_cap;

        /* ensure powers of 4 up to 4^(n-1) are pre‑computed   */
        if (pwr_cap <= (idx_t)n - 1) {
            size_t new_pwr_cap = (size_t)n;          /* indices 0 .. n-1   */
            seq_t *tmp2 = (seq_t *)realloc(pwr, new_pwr_cap * sizeof(seq_t));
            if (!tmp2) return 0;
            pwr = tmp2;
            for (idx_t i = pwr_cap; i < (idx_t)new_pwr_cap; ++i)
                pwr[i] = pwr[i - 1] * (seq_t)4;
            pwr_cap = (idx_t)new_pwr_cap;
        }

        /* compute missing a(k) values from old_cap up to n   */
        for (idx_t k = old_cap; k <= n; ++k) {
            if (k == 0) {   /* a(0) already known */
                continue;
            }
            seq_t sum = 0;
            for (idx_t i = 0; i <= k - 1; ++i) {
                idx_t j = k - 1 - i;
                seq_t term = pwr[i] * seq[i];
                term *= seq[j];
                sum += term;
            }
            seq[k] = sum;
        }
    }

    return seq[n];
}
