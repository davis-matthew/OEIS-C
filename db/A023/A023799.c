/*
 * A023799  Xenodromes: all digits in base 4 are different.
 * https://oeis.org/A023799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   a(n) = the nth nonnegative integer whose base‑4 representation
 *            contains no repeated digits
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   distinct_base4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/distinct_base4.h"

/*
 * Build the sequence once.  The maximum value in the sequence is
 * the largest 4‑digit number with distinct digits: 3210_4 = 228.
 */
static seq_t cache[64];
static bool initialized = false;
static size_t term_count = 0;

static void build_cache(void)
{
    if (initialized) return;
    seq_t num = 0;
    while (num <= (seq_t)228) {
        if (distinct_base4(num)) {
            cache[term_count++] = num;
        }
        num++;
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    build_cache();
    if ((size_t)(n - 1) < term_count)
        return cache[n - 1];
    /* Undefined for n beyond the length of the sequence. */
    return 0;
}
