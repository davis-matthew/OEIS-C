/*
 * A219590  Number of n X 3 arrays of the minimum value of corresponding
 *          elements and their horizontal or vertical neighbors in a random,
 *          but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..2 n X 3 array.
 * https://oeis.org/A219590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 32, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(1)=6, a(2)=18, a(3)=84, a(4)=264, a(5)=705, a(6)=1739,
 *   a(7)=4129, a(8)=9518, a(9)=21271, a(10)=46019, a(11)=96412,
 *   a(12)=195879, a(13)=386603, a(14)=742477.
 *
 *   For n > 14:
 *     a(n) = 12*a(n‑1) - 66*a(n‑2) + 220*a(n‑3) - 495*a(n‑4)
 *           + 792*a(n‑5) - 924*a(n‑6) + 792*a(n‑7) - 495*a(n‑8)
 *           + 220*a(n‑9) - 66*a(n‑10) + 12*a(n‑11) - a(n‑12)
 */

#include <stdlib.h>

static const seq_t seeds[14] = {
    (seq_t)6,   (seq_t)18,  (seq_t)84,   (seq_t)264,
    (seq_t)705, (seq_t)1739, (seq_t)4129,  (seq_t)9518,
    (seq_t)21271,(seq_t)46019,(seq_t)96412,(seq_t)195879,
    (seq_t)386603,(seq_t)742477
};

static seq_t *cache = NULL;
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Direct lookup if already cached */
    if (n <= cache_len)
        return cache[n - 1];

    /* Initialize cache with the first 14 terms */
    if (cache == NULL) {
        cache_len = 14;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return (seq_t)0; /* out of memory: return 0 */
        for (size_t i = 0; i < 14; ++i)
            cache[i] = seeds[i];
    }

    /* Extend cache up to requested n */
    if ((size_t)n > cache_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)n);
        if (!new_cache) return (seq_t)0; /* allocation failure: return 0 */
        cache = new_cache;
        for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
            size_t idx = i - 1;          /* zero‑based index of a(i) */
            /* Compute positive and negative contributions separately to avoid underflow */
            seq_t pos = 12 * cache[idx - 1]
                      + 220 * cache[idx - 3]
                      + 792 * cache[idx - 5]
                      + 792 * cache[idx - 7]
                      + 220 * cache[idx - 9]
                      + 12 * cache[idx -11];
            seq_t neg = 66 * cache[idx - 2]
                      + 495 * cache[idx - 4]
                      + 924 * cache[idx - 6]
                      + 495 * cache[idx - 8]
                      + 66  * cache[idx -10]
                      +      cache[idx -12];
            cache[idx] = pos - neg;       /* pos >= neg for a(n) >= 0 */
        }
        cache_len = n;
    }

    return cache[n - 1];
}
