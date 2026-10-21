/*
 * A203731  Number of (n+1)X5 0..7 arrays with every 2X2 subblock having equal
 *          diagonal elements or equal antidiagonal elements.
 * https://oeis.org/A203731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 141*a(n‑1)+34391*a(n‑2)−1914087*a(n‑3) *
 *            −5360404*a(n‑4)+185626112*a(n‑5)+226020536*a(n‑6) *
 *            −801895584*a(n‑7); * Initial values: * a(1)=3240000 *
 *            a(2)=681461824 * a(3)=161916229056 * a(4)=39986164706496 *
 *            a(5)=9889622728751424 * a(6)=2456635775400515136 *
 *            a(7)=609221928288391040960 * * Th...
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 6, u128 n <= 14,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;   /* Out of range */
    }

    /* Recurrence coefficients */
    const seq_t C1 = (seq_t)141;
    const seq_t C2 = (seq_t)34391;
    const seq_t C3 = (seq_t)1914087;
    const seq_t C4 = (seq_t)5360404;
    const seq_t C5 = (seq_t)185626112;
    const seq_t C6 = (seq_t)226020536;
    const seq_t C7 = (seq_t)801895584;

    /* Cached values of a(1)..a(n) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Initialise cache only once */
    if (!cache) {
        cache_size = 8;               /* indices 0..7 (0 unused) */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (size_t i = 0; i < cache_size; ++i) {
            cache[i] = (seq_t)0;
        }
        /* Initial seven terms */
        cache[1] = seq_from_str("3240000");
        cache[2] = seq_from_str("681461824");
        cache[3] = seq_from_str("161916229056");
        cache[4] = seq_from_str("39986164706496");
        cache[5] = seq_from_str("9889622728751424");
        cache[6] = seq_from_str("2456635775400515136");
        cache[7] = seq_from_str("609221928288391040960");
    }

    /* Grow cache if required */
    size_t idx = (size_t)n;
    if (idx >= cache_size) {
        size_t new_size = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) { /* Allocation failed */
            return (seq_t)0;
        }
        for (size_t i = cache_size; i < new_size; ++i) {
            tmp[i] = (seq_t)0;
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Return cached value if already computed */
    if (cache[idx] != (seq_t)0) {
        return cache[idx];
    }

    /* Compute values up to idx using recurrence */
    for (size_t m = 8; m <= idx; ++m) {
        seq_t value = (seq_t)0;
        value += C1 * cache[m-1];
        value += C2 * cache[m-2];
        value -= C3 * cache[m-3];
        value -= C4 * cache[m-4];
        value += C5 * cache[m-5];
        value += C6 * cache[m-6];
        value -= C7 * cache[m-7];
        cache[m] = value;
    }

    return cache[idx];
}
