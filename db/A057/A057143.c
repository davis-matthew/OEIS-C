/*
 * A057143  Largest of the most frequently occurring numbers in 1-to-n
 *          multiplication table.
 * https://oeis.org/A057143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Largest product in the n×n multiplication table having
 *            the maximum frequency among all products. The algorithm fills an
 *            array of 16‑bit counters (one per possible product) and finds
 *            the product with the largest count, returning the largest such
 *            product
 * Range:     u8 n <= 44, u16 n <= 1759, u32 n <= 9487, u64 n <= 9487,
 *            u128 n <= 9487, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Frequency counters for products.  */
static uint16_t *freq = NULL;
static size_t    freq_size = 0;

/* Sequence entry.  */
seq_t a(idx_t n)
{
    /* Handle the offset domain explicitly.  */
    if (n == 0) return (seq_t)0;

    /* Determine the maximum product (n*n) and the required array size.  */
    idx_t prod_limit = n * n;
    size_t needed = (size_t)prod_limit + 1;   /* index 0 unused */

    /* Allocate or resize the frequency array.  */
    if (freq_size < needed) {
        uint16_t *tmp = (uint16_t*)calloc(needed, sizeof(uint16_t));
        if (!tmp) abort();
        free(freq);
        freq = tmp;
        freq_size = needed;
    } else {
        /* Reuse existing array but clear only the needed part.  */
        memset(freq, 0, needed * sizeof(uint16_t));
    }

    /* Count occurrences of every product i*j.  */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = 1; j <= n; ++j) {
            idx_t p = i * j;
            ++freq[p];
        }
    }

    /* Find the product with maximum frequency, breaking ties by choosing the larger product.  */
    uint16_t maxcnt = 0;
    seq_t best = 0;
    for (idx_t p = 1; p <= prod_limit; ++p) {
        uint16_t c = freq[p];
        if (c > maxcnt || (c == maxcnt && p > best)) {
            maxcnt = c;
            best = (seq_t)p;
        }
    }

    return best;
}
