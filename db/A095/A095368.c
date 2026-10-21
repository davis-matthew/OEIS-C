/*
 * A095368  Number of walks of length n between two nodes at distance 3 in the
 *          cycle graph C_9.
 * https://oeis.org/A095368
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+5a(n-2)-4a(n-3)-5a(n-4)+2a(n-5) for n >= 8 *
 *            Initial values: a(3)=1, a(4)=0, a(5)=5, a(6)=1, a(7)=21 *
 *            Sequence offset: 3 (a(3) is first term)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* values for n < 3 are undefined by the OEIS entry;
       return 0 for simplicity. */
    if (n < 3)
        return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* Ensure the cache is large enough. */
    if ((size_t)n >= cap) {
        size_t new_cap = n + 1;
        /* grow to at least new_cap, doubling size to keep amortized cost O(1) */
        if (cap == 0) {
            cap = 16;
            cache = (seq_t *)malloc(cap * sizeof(seq_t));
        } else {
            while (cap <= (size_t)n)
                cap <<= 1;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }
        /* initialise base values up to 7 on first expansion */
        if (n >= 3 && cache[3] == 0) { /* simple flag check */
            cache[3] = (seq_t)1;
            cache[4] = (seq_t)0;
            cache[5] = (seq_t)5;
            cache[6] = (seq_t)1;
            cache[7] = (seq_t)21;
        }
    }

    /* Compute terms on demand */
    if ((size_t)n > 7) {
        for (idx_t k = 8; k <= n; ++k) {
            /* compute positive and negative parts separately to avoid unsigned wrap */
            seq_t pos = cache[k - 1]
                      + (seq_t)5 * cache[k - 2]
                      + (seq_t)2 * cache[k - 5];
            seq_t neg = (seq_t)4 * cache[k - 3]
                      + (seq_t)5 * cache[k - 4];
            cache[k] = pos - neg;   /* unsigned subtraction is safe here */
        }
    }

    return cache[n];
}
