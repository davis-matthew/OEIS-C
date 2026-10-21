/*
 * A219367  Number of n X 2 arrays of the minimum value of corresponding
 *          elements and their horizontal or vertical neighbors in a random,
 *          but sorted with lexicographically nondecreasing rows and columns,
 *          0..3 n X 2 array.
 * https://oeis.org/A219367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the recurrence a(1) = 4, a(2) = 13, … , a(9) =
 *            11665 a(10) = 9*a(9) – 36*a(8) + 84*a(7) – 126*a(6) + 126*a(5) –
 *            84*a(4) + 36*a(3) – 9*a(2) + a(1) – 6 for n > 10: a(n) =
 *            9*a(n‑1) – 36*a(n‑2) + 84*a(n‑3) – 126*a(n‑4) + 126*a(n‑5) –
 *            84*a(n‑6) + 36*a(n‑7) – 9*a(n‑8) + a(n‑9)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 51, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Compute the n-th term of OEIS sequence A219367. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset 1 */

    /* static cache: elements 0..len-1 correspond to a(1)..a(len) */
    static seq_t *cache = NULL;
    static size_t len = 0;            /* number of terms stored */
    static size_t cap = 0;            /* capacity of cache */

    /* Ensure enough space. */
    if (!cache) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failed */
        memset(cache, 0, cap * sizeof(seq_t));
        /* initial 9 values */
        static const uint64_t init_vals[9] = {
            4ULL, 13ULL, 60ULL, 193ULL, 514ULL,
            1231ULL, 2748ULL, 5802ULL, 11665ULL
        };
        for (size_t i = 0; i < 9; ++i) cache[i] = (seq_t)init_vals[i];
        len = 9;
    }

    size_t target = (size_t)n;
    if (target > cap) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap < target) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        /* zero the newly allocated part */
        memset(tmp + cap, 0, (new_cap - cap) * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to target. */
    for (size_t i = len + 1; i <= target; ++i) {   /* i is 1‑based index */
        seq_t val;
        if (i <= 9) {
            /* already initialized */
            continue;
        } else if (i == 10) {
            /* a(10) uses the constant term –6 from the numerator. */
            val = (seq_t)9 * cache[8]               // 9*a9
                - (seq_t)36 * cache[7]              // –36*a8
                + (seq_t)84 * cache[6]              // +84*a7
                - (seq_t)126 * cache[5]             // –126*a6
                + (seq_t)126 * cache[4]             // +126*a5
                - (seq_t)84 * cache[3]              // –84*a4
                + (seq_t)36 * cache[2]              // +36*a3
                - (seq_t)9 * cache[1]               // –9*a2
                + cache[0]                          // +a1
                - (seq_t)6;                         // –6
        } else {
            /* homogeneous recurrence for n > 10. */
            val = (seq_t)9   * cache[i-2]                // 9*a(i‑1)
                - (seq_t)36 * cache[i-3]                // –36*a(i‑2)
                + (seq_t)84 * cache[i-4]                // +84*a(i‑3)
                - (seq_t)126 * cache[i-5]               // –126*a(i‑4)
                + (seq_t)126 * cache[i-6]               // +126*a(i‑5)
                - (seq_t)84 * cache[i-7]                // –84*a(i‑6)
                + (seq_t)36 * cache[i-8]                // +36*a(i‑7)
                - (seq_t)9  * cache[i-9]                // –9*a(i‑8)
                + cache[i-10];                          // +a(i‑9)
        }
        cache[i-1] = val;
    }
    len = target;
    return cache[n-1];
}
