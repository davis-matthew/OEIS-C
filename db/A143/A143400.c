/*
 * A143400  Expansion of x^k/Product_{t=k..2k} (1-tx) for k=5.
 * https://oeis.org/A143400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A143400: expansion of x^5 / ((1‑5x)(1‑6x)(1‑7x)(1‑8x)(1‑9x)(1‑10x))
   The coefficients satisfy the linear recurrence
       a(n) = 45·a(n‑1) – 835·a(n‑2) + 8175·a(n‑3) – 44524·a(n‑4)
              + 127860·a(n‑5) – 151200·a(n‑6)   for n > 5,
   with initial values a(0)=a(1)=a(2)=a(3)=a(4)=0 and a(5)=1.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Expand cache to include index n */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        /* realloc needs a cast in C++ */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failure – return 0 as a safe fallback */
            return (seq_t)0;
        }
        cache = new_cache;

        /* Initialise new entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 5) {
                cache[i] = (seq_t)0;          /* a(0)..a(4) = 0 */
            } else if (i == 5) {
                cache[i] = (seq_t)1;          /* a(5) = 1 */
            } else {                          /* recurrence for i > 5 */
                seq_t v = (seq_t)45   * cache[i - 1];
                v -= (seq_t)835   * cache[i - 2];
                v += (seq_t)8175  * cache[i - 3];
                v -= (seq_t)44524 * cache[i - 4];
                v += (seq_t)127860 * cache[i - 5];
                v -= (seq_t)151200 * cache[i - 6];
                cache[i] = v;
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
