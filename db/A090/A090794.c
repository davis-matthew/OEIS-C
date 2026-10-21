/*
 * A090794  Number of partitions of n such that the number of different parts
 *          is odd.
 * https://oeis.org/A090794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p(n) - D(n)) / 2 where p(n) is the partition number and
 *            D(n) is the coefficient of x^n in D(x) = ∏_{k≥1} (1 - 2x^k/(1 -
 *            x^k)) = (∏_{k≥1} (1 - 2x^k)) * p(x)
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Each new factor (1 - 2x^k) gives the simple recurrence
   D_new(n) = D_old(n) - 2·D_old(n - k)   (for n ≥ k)
   Thus D can be built in O(n^2) using only additions/subtractions. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *p = NULL;        /* partition numbers */
    static seq_t *D = NULL;        /* D(n) values   */
    static idx_t max_n = 0;        /* largest n pre‑computed */

    if (n == 0) return 0;          /* offset is 1 – not used */

    /* Grow pre‑computed tables if necessary */
    if (n > max_n) {
        idx_t new_max = n;
        seq_t *p_new = (seq_t*)calloc(new_max + 1, sizeof(seq_t));
        seq_t *D_new = (seq_t*)calloc(new_max + 1, sizeof(seq_t));

        /* ---------- 1. Partition numbers p(n) ----------
           p[0] = 1, p[s] = Σ_{k=1}^s p[s - k]           */
        p_new[0] = 1;
        for (idx_t k = 1; k <= new_max; ++k) {
            for (idx_t s = k; s <= new_max; ++s) {
                p_new[s] += p_new[s - k];
            }
        }

        /* ---------- 2. Initialize D[n] = p[n] ----------
            (the product starts with factor 1)             */
        for (idx_t i = 0; i <= new_max; ++i)
            D_new[i] = p_new[i];

        /* ---------- 3. Apply recurrence for each factor (1 - 2x^k)  ----------
           D[n] = D[n] - 2 * D[n - k]   (process indices descending) */
        for (idx_t k = 1; k <= new_max; ++k) {
            for (idx_t s = new_max; s >= k; --s) {
                D_new[s] -= D_new[s - k] * 2;
            }
        }

        /* Replace old tables */
        free(p); free(D);
        p = p_new;
        D = D_new;
        max_n = new_max;
    }

    /* ---------- 4. Result a(n) = (p(n) - D(n)) / 2  ---------- */
    seq_t diff = p[n] - D[n];
    return diff / 2;
}
