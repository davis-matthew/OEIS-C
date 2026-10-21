/*
 * A225527  Number of distinct products i*j over all pairs (i,j) with i,j>=0
 *          and i+j <= n.
 * https://oeis.org/A225527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of distinct products i*j for i,j >= 0 with i+j ≤ n
 *            (computed by enumerating all pairs and deduplicating with a
 *            dynamic bitset)
 * Range:     u8 n <= 43, u16 n <= 400, u32 n <= 400, u64 n <= 400,
 *            u128 n <= 400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Computes a(n) for OEIS sequence A225527.
   seq_t and idx_t are defined by the build environment. */
seq_t a(idx_t n)
{
    /* Static state shared between calls */
    static idx_t      processed_n = (idx_t)-1;  /* largest n computed so far */
    static size_t     cache_capacity = 0;
    static seq_t     *a_cache = NULL;
    static seq_t       product_count = 0;        /* number of distinct products seen */

    /* Bitset for set of products */
    static size_t     bitset_size = 0;          /* number of bits currently allocated */
    static uint64_t  *bits = NULL;

    /* Initialize for n == 0, if necessary */
    if (processed_n == (idx_t)-1) {
        processed_n = 0;
        cache_capacity = 1;
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        a_cache[0] = (seq_t)1;          /* a(0) = 1 (only product 0) */

        product_count = (seq_t)1;       /* product 0 seen once */

        /* Allocate bitset for a single bit (0) */
        bitset_size = 1;
        bits = (uint64_t *)malloc(sizeof(uint64_t));
        bits[0] = 1ull;                 /* bit 0 set */
    }

    /* Return cached value if already computed */
    if (n <= processed_n)
        return a_cache[n];

    /* Grow cache array if needed */
    if ((size_t)n + 1 > cache_capacity) {
        cache_capacity = (size_t)n + 1;
        a_cache = (seq_t *)realloc(a_cache, cache_capacity * sizeof(seq_t));
    }

    /* Enumerate new pairs with i + j = k for each k from processed_n+1 to n */
    for (idx_t k = processed_n + 1; k <= n; ++k) {
        for (idx_t i = 0; i <= k; ++i) {
            idx_t j = k - i;
            /* Compute product as 128-bit to avoid overflow and cast back to idx_t */
            idx_t prod = (idx_t)((unsigned __int128)i * j);

            /* Ensure bitset covers index prod */
            if ((size_t)prod + 1 > bitset_size) {
                size_t old_words = (bitset_size + 63) >> 6;
                size_t new_bits   = (size_t)prod + 1;
                size_t new_words  = (new_bits + 63) >> 6;
                bits = (uint64_t *)realloc(bits, new_words * sizeof(uint64_t));
                if (new_words > old_words)
                    memset(bits + old_words, 0, (new_words - old_words) * sizeof(uint64_t));
                bitset_size = new_bits;
            }

            size_t word = (size_t)prod >> 6;
            uint64_t mask = 1ull << (prod & 63);

            if ((bits[word] & mask) == 0) {
                bits[word] |= mask;
                ++product_count;
            }
        }
        a_cache[k] = product_count;
    }

    processed_n = n;
    return a_cache[n];
}
