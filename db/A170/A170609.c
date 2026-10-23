/*
 * A170609  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 * Recurrence: * for 1 ≤ n ≤ 47 : a(n) = 2 + 22 *
 *            Σ_{k=0}^{n-1} a(k) * a(48) : a(48) = 1 + 22 * Σ_{k=1}^{47} a(k)
 *            – 253 * a(0) * for n ≥ 49 : a(n) = 1 + 22 * Σ_{k=n-47}^{n-1}
 *            a(k) – 253 * a(n-48) * * The implementation keeps a cache and
 *            maintains running sums to * evaluate the recurrence...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, exit */
#include <stdbool.h>  /* bool */

seq_t a(idx_t n)
{
    /* ---------- static cache ------------------------------------------- */
    static seq_t *cache      = NULL;   /* array of a(i) */
    static idx_t cache_len   = 0;      /* number of entries stored */
    static seq_t total_sum   = 0;      /* Σ_{k=0}^{i-1} a(k) for i < 48   */
    static seq_t window_sum  = 0;      /* Σ_{k=i-47}^{i-1} a(k) for i ≥ 48 */
    static bool  initialized = false;

    /* ---------- initialisation ------------------------------------------ */
    if (!initialized) {
        cache = (seq_t *)malloc(sizeof(seq_t));          /* will hold a(0) */
        if (!cache) exit(1);
        cache[0]  = (seq_t)1;
        total_sum = cache[0];
        cache_len = 1;
        initialized = true;
    }

    /* ---------- grow cache if a larger n is requested ------------------- */
    if ((idx_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;

        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val;

            if (i <= 47) {                         /* 1 ≤ i ≤ 47          */
                val = (seq_t)2 + (seq_t)22 * total_sum;
                cache[i] = val;
                total_sum += val;                  /* update total_sum    */
            }
            else if (i == 48) {                    /* i = 48              */
                window_sum = total_sum - cache[0];
                val = (seq_t)1 + (seq_t)22 * window_sum
                      - (seq_t)253 * cache[0];
                cache[i] = val;
                window_sum = window_sum - cache[1] + val;
                total_sum += val;
            }
            else {                                 /* i ≥ 49              */
                val = (seq_t)1 + (seq_t)22 * window_sum
                      - (seq_t)253 * cache[i - 48];
                cache[i] = val;
                window_sum = window_sum - cache[i - 47] + val;
                total_sum += val;
            }
        }
        cache_len = n + 1;
    }

    /* ---------- return the requested term ------------------------------ */
    return cache[n];
}
