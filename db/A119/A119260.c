/*
 * A119260  Numbers with even decimal digits in increasing order.
 * https://oeis.org/A119260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generate numbers with even decimal digits in strictly increasing order.
   The sequence is finite: 0, 2, 4, 6, 8, 24, 26, 28, 46, 48, 68,
   246, 248, 268, 468, 2468. The function builds this list on first use
   and returns the *n*-th term (1‑based).  For n larger than the list
   length it returns 0. */

#include <stddef.h>
#include <stdbool.h>

/* Persistent cache for the 16 sequence values.  */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static bool initialized = false;

static void generate_cache(void)
{
    const uint8_t even_digits[4] = {2, 4, 6, 8};
    size_t i, j, s;
    size_t total = 1; /* include subset 0 -> number 0 */

    /* Count all non‑empty subsets of four digits. */
    for (i = 1; i < 16; ++i) {
        ++total;
    }

    /* Allocate the cache. */
    cache = (seq_t *)malloc(total * sizeof(seq_t));
    cache_len = total;

    /* Subset 0 corresponds to the number 0. */
    cache[0] = (seq_t)0;

    /* Enumerate all non‑empty subsets. */
    idx_t pos = 1;
    for (s = 1; s < 16; ++s) {
        seq_t val = (seq_t)0;
        /* Build number from the selected digits (in increasing order). */
        for (i = 0; i < 4; ++i) {
            if (s & (1u << i)) {
                val = val * (seq_t)10 + (seq_t)even_digits[i];
            }
        }
        cache[pos++] = val;
    }

    /* Simple insertion sort to put numbers in ascending order. */
    for (i = 1; i < cache_len; ++i) {
        if (cache[i] < cache[i - 1]) {
            seq_t key = cache[i];
            j = i;
            while (j > 0 && cache[j - 1] > key) {
                cache[j] = cache[j - 1];
                j--;
            }
            cache[j] = key;
        }
    }
}

seq_t a(idx_t n)
{
    /* 1‑based index: n >= 1 */
    if (!initialized) {
        generate_cache();
        initialized = true;
    }

    if (n == 0) return 0; /* undefined, return 0 */

    if (n <= cache_len) {
        return cache[n - 1];
    } else {
        /* Out of bounds – the sequence ends here. */
        return (seq_t)0;
    }
}
