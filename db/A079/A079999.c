/*
 * A079999  Number of permutations (p(1),p(2),...,p(n)) of (1,2,...,n) such
 *          that p(i)-i is in {-2,-1,3} for all i=1,...,n.
 * https://oeis.org/A079999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 38, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = a(n-3) + a(n-4) + 2*a(n-5) + a(n-6) - a(n-8) - a(n-10)   for n >= 10
   Initial values:
   a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=1, a(6)=1,
   a(7)=1, a(8)=1, a(9)=4
   The sequence is computed by dynamic programming with memoisation. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;          /* number of elements stored */

    /* Allocate or grow cache if needed */
    if (n >= cached_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)                                   /* allocation failed */
            return 0;                                     /* fall back to 0 */
        cache = new_cache;

        /* Initialise new positions */
        for (idx_t i = cached_size; i <= n; ++i) {
            if (i < 10) {
                switch ((int)i) {
                    case 0: cache[i] = 1; break;
                    case 1: cache[i] = 0; break;
                    case 2: cache[i] = 0; break;
                    case 3: cache[i] = 0; break;
                    case 4: cache[i] = 1; break;
                    case 5: cache[i] = 1; break;
                    case 6: cache[i] = 1; break;
                    case 7: cache[i] = 1; break;
                    case 8: cache[i] = 1; break;
                    case 9: cache[i] = 4; break;
                }
            } else {
                seq_t a3  = cache[i-3];
                seq_t a4  = cache[i-4];
                seq_t a5  = cache[i-5];
                seq_t a6  = cache[i-6];
                seq_t a8  = cache[i-8];
                seq_t a10 = cache[i-10];
                cache[i] = a3 + a4 + 2*a5 + a6 - a8 - a10;
            }
        }
        cached_size = new_size;
    }

    return cache[n];
}
