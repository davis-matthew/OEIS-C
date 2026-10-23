/*
 * A107854  G.f. x*(x^2+1)*(x^3-x-1)/((2*x^3+x^2-1)*(x^4+1)).
 * https://oeis.org/A107854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0,a(1)=1,a(2)=1,a(3)=2,a(4)=3,a(5)=3,a(6)=5; 
   for n>=7: a(n)=a(n-2)+2*a(n-3)-a(n-4)+a(n-6)+2*a(n-7) */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        /* grow cache to hold up to index n */
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t*)malloc(sizeof(seq_t) * new_size);
        if (!new_cache) return 0;  /* OOM, unlikely in tests */

        /* copy old values */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;

        /* compute missing entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            switch (i) {
                case 0: cache[i] = 0; break;
                case 1: cache[i] = 1; break;
                case 2: cache[i] = 1; break;
                case 3: cache[i] = 2; break;
                case 4: cache[i] = 3; break;
                case 5: cache[i] = 3; break;
                case 6: cache[i] = 5; break;
                default:
                    /* recurrence: a(n)=a(n-2)+2*a(n-3)-a(n-4)+a(n-6)+2*a(n-7) */
                    {
                        seq_t sum = cache[i-2]
                                + 2 * cache[i-3];
                        sum -= cache[i-4];
                        sum += cache[i-6];
                        sum += 2 * cache[i-7];
                        cache[i] = sum;
                    }
                    break;
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
