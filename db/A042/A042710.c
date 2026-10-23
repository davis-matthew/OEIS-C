/*
 * A042710  Numerators of continued fraction convergents to sqrt(885).
 * https://oeis.org/A042710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042710 - Numerators of continued fraction convergents to sqrt(885)
   Recurrence: a(n) = 238*a(n-4) - a(n-8)
   Initial values: a(0)=29, a(1)=30, a(2)=89, a(3)=119,
                   a(4)=6991, a(5)=7110, a(6)=21211, a(7)=28321
*/

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static unsigned char *present = NULL;
static size_t cache_cap = 0;
static bool init_done = false;

static const seq_t init_vals[8] = {
    29, 30, 89, 119, 6991, 7110, 21211, 28321
};

/* Ensure the internal cache has room for indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (needed <= cache_cap)
        return;
    size_t new_cap = needed;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    present = (unsigned char *)realloc(present, new_cap * sizeof(unsigned char));
    for (size_t i = cache_cap; i < new_cap; ++i)
        present[i] = 0;
    cache_cap = new_cap;
}

/* Initialise the first eight terms once. */
static void initialise(void)
{
    if (init_done)
        return;
    ensure_capacity(7);
    for (size_t i = 0; i < 8; ++i) {
        cache[i] = init_vals[i];
        present[i] = 1;
    }
    init_done = true;
}

seq_t a(idx_t n)
{
    initialise();

    if (n < 8)
        return cache[(size_t)n];

    ensure_capacity(n);

    if (present[(size_t)n])
        return cache[(size_t)n];

    /* Compute all missing terms up to n. */
    for (idx_t i = 8; i <= n; ++i) {
        if (!present[(size_t)i]) {
            cache[(size_t)i] = (seq_t)238 * cache[(size_t)(i - 4)]
                               - cache[(size_t)(i - 8)];
            present[(size_t)i] = 1;
        }
    }
    return cache[(size_t)n];
}
