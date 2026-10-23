/*
 * A020668  Numbers of the form x^2 + 4*y^2.
 * https://oeis.org/A020668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest nth positive integer of the form x^2 + 4*y^2
 *            where x, y ≥ 0 (0 counts as a valid term, first entry). The
 *            sequence is cached lazily; the array is resized only when a
 *            larger n is requested
 * Range:     u8 n <= 72, u16 n <= 7500, u32 n <= 7500, u64 n <= 7500,
 *            u128 n <= 7500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;     /* array of computed terms, sorted, 0‑based */
static size_t cached_len = 0;   /* number of terms stored */

/* Ensure that the cache contains at least 'needed' terms.
   Uses a simple bound strategy: generate all numbers <= bound,
   deduplicate them, and keep increasing bound until enough terms
   are obtained.  The bound is chosen as 6*needed+100 and doubled
   if necessary.  Complexity: O(bound). */
static void ensure_cache(size_t needed)
{
    if (needed <= cached_len) return;

    size_t bound = needed * 6 + 100;     /* initial guess */
    while (true) {
        /* mark array for numbers up to 'bound' */
        size_t mark_size = bound + 1U;
        unsigned char *mark = (unsigned char*)calloc(mark_size, 1);
        if (!mark) abort();              /* memory allocation failure */

        /* enumerate all x^2 + 4*y^2 <= bound */
        size_t y = 0;
        while (1) {
            uint64_t base = (uint64_t)4 * y * y;
            if (base > bound) break;
            size_t x = 0;
            while (1) {
                uint64_t val = base + (uint64_t)x * x;
                if (val > bound) break;
                mark[(size_t)val] = 1;
                ++x;
            }
            ++y;
        }

        /* count how many distinct values we have */
        size_t count = 0;
        for (size_t i = 0; i <= bound; ++i)
            if (mark[i]) ++count;

        if (count >= needed) {
            /* build the cache array in sorted order */
            seq_t *new_cache = (seq_t*)malloc(count * sizeof(seq_t));
            if (!new_cache) abort();
            size_t pos = 0;
            for (size_t i = 0; i <= bound; ++i)
                if (mark[i]) new_cache[pos++] = (seq_t)i;

            free(mark);
            free(cache);
            cache = new_cache;
            cached_len = count;
            break;
        }

        free(mark);
        bound *= 2;        /* increase bound and retry */
    }
}

/* Public function requested by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* safeguard for invalid input */
    ensure_cache((size_t)n);
    return cache[(size_t)n - 1];      /* a(1) is at index 0 */
}
