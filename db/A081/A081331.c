/*
 * A081331  Number of numbers <= n that can be written as sum of two 3-smooth
 *          numbers.
 * https://oeis.org/A081331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 431, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A081331 – Number of integers ≤ n that can be written as
 * the sum of two 3‑smooth numbers (numbers of the form 2^i·3^j).
 *
 * This implementation first generates all 3‑smooth numbers ≤ n,
 * then marks all pairwise sums that do not exceed n, and finally
 * counts how many integers in [1,n] have been marked.
 *
 * The algorithm runs in O(k² + n) time where k is the number of
 * 3‑smooth numbers up to n (k = O((log n)²)), and needs O(n)
 * memory for the boolean array.  For the required limits (n up to
 * ~10 000) this comfortably stays below the 2 s time limit.
 */

#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* Count of 3‑smooth numbers ≤ n (k). */
    idx_t k = 0;
    for (idx_t p2 = 1; p2 <= n; p2 *= 2) {
        for (idx_t p3 = 1; p3 <= n / p2; p3 *= 3) {
            k++;
        }
    }

    /* Collect the 3‑smooth numbers into an array. */
    idx_t *smooth = (idx_t *)malloc(k * sizeof(idx_t));
    if (!smooth) return 0;  /* allocation failed – return 0 (unlikely in tests) */

    idx_t idx = 0;
    for (idx_t p2 = 1; p2 <= n; p2 *= 2) {
        for (idx_t p3 = 1; p3 <= n / p2; p3 *= 3) {
            smooth[idx++] = p2 * p3;
        }
    }

    /* Boolean array marking representable sums: rep[s] == 1 if s can be written
       as sum of two 3‑smooth numbers. */
    size_t rep_size = (size_t)(n + 1);   /* n+1 because we use indices 0..n */
    char *rep = (char *)calloc(rep_size, sizeof(char));
    if (!rep) { free(smooth); return 0; }

    /* Mark all pairwise sums. */
    for (idx_t i = 0; i < k; ++i) {
        for (idx_t j = 0; j < k; ++j) {
            idx_t s = smooth[i] + smooth[j];
            if (s <= n) rep[s] = 1;
        }
    }

    /* Count representable integers in [1, n]. */
    seq_t count = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (rep[i]) count++;
    }

    free(rep);
    free(smooth);
    return count;
}
