/*
 * A135137  Numbers that are the sum of three numbers from the set {1, 5, 10,
 *          20, 50, 100}.
 * https://oeis.org/A135137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A135137: sums of three numbers from the set {1,5,10,20,50,100},
   stored in increasing order.  a(1) = 3, a(2) = 7, a(3) = 11, …   */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static const seq_t VALUES[6] = { 1, 5, 10, 20, 50, 100 };

/* cache of distinct sums */
static seq_t *CACHE = NULL;
static size_t CACHE_LEN = 0;

seq_t a(idx_t n) {
    /* lazily compute the cache on first call */
    if (CACHE_LEN == 0) {
        /* values lie between 3 and 300 inclusive */
        bool seen[301] = { false };
        for (size_t i = 0; i < 6; ++i) {
            for (size_t j = i; j < 6; ++j) {
                for (size_t k = j; k < 6; ++k) {
                    int sum = (int)VALUES[i] + (int)VALUES[j] + (int)VALUES[k];
                    seen[sum] = true;
                }
            }
        }
        /* count and allocate cache */
        size_t count = 0;
        for (int s = 3; s <= 300; ++s)
            if (seen[s]) ++count;
        /* allocate all at once */
        CACHE = (seq_t *)malloc(count * sizeof(seq_t));
        if (!CACHE) abort();  /* allocation failure */
        size_t idx = 0;
        for (int s = 3; s <= 300; ++s)
            if (seen[s])
                CACHE[idx++] = (seq_t)s;
        CACHE_LEN = count;
    }

    /* return the n-th term if it exists, otherwise 0 */
    if (n == 0 || n > CACHE_LEN)
        return (seq_t)0;
    return CACHE[n - 1];
}
