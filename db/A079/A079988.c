/*
 * A079988  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=3, r=3, I={0,1,2}.
 * https://oeis.org/A079988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 24, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = a(n-2) + a(n-3) + a(n-4) + 4*a(n-5) + 4*a(n-6)
          - a(n-7) - 3*a(n-8) - 2*a(n-9) + a(n-10)
          - 4*a(n-11) - 3*a(n-12) + a(n-13) + 4*a(n-14)
          - a(n-16) + a(n-17) + a(n-18) - a(n-20)
   Initial values:
   a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=3, a(6)=3, a(7)=3,
   a(8)=6, a(9)=10, a(10)=29, a(11)=39, a(12)=61, a(13)=101,
   a(14)=179, a(15)=335, a(16)=566, a(17)=928, a(18)=1575, a(19)=2705 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;

    /* The first 20 terms are hard‑coded. */
    static const seq_t base[20] = {
          1ULL, 0ULL, 0ULL, 0ULL, 1ULL,
          3ULL, 3ULL, 3ULL, 6ULL, 10ULL,
         29ULL, 39ULL, 61ULL,101ULL,179ULL,
        335ULL,566ULL,928ULL,1575ULL,2705ULL
    };

    /* Initialise cache with base values. */
    if (!cache) {
        cache = (seq_t *)malloc(20 * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* unlikely, but defensive */
        for (idx_t i = 0; i < 20; ++i) cache[i] = base[i];
        max_computed = 19;
    }

    if (n > max_computed) {
        /* Grow cache to accommodate up to n. */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure, unlikely */
        cache = tmp;

        /* Compute missing terms sequentially. */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t sum = 0ULL;
            /* Positive contributions */
            sum += cache[i-2];
            sum += cache[i-3];
            sum += cache[i-4];
            sum += cache[i-5] * (seq_t)4;
            sum += cache[i-6] * (seq_t)4;
            sum += cache[i-10];
            sum += cache[i-13];
            sum += cache[i-14] * (seq_t)4;
            sum += cache[i-17];
            sum += cache[i-18];
            /* Negative contributions */
            sum -= cache[i-7];
            sum -= cache[i-8] * (seq_t)3;
            sum -= cache[i-9] * (seq_t)2;
            sum -= cache[i-11] * (seq_t)4;
            sum -= cache[i-12] * (seq_t)3;
            sum -= cache[i-16];
            sum -= cache[i-20];
            cache[i] = sum;
        }
        max_computed = n;
    }

    return cache[n];
}
