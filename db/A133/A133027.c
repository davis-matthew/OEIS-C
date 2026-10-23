/*
 * A133027  Divisors of 1701.
 * https://oeis.org/A133027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy
 * Formula:   a(n) is the n‑th divisor of 1701 (seen as 3^5 · 7) in ascending
 *            order. Only 12 divisors exist
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* for size_t */
#include <stdbool.h>     /* for bool */

static seq_t div_cache[12];
static bool div_cache_ready = false;

/* Helper: swap two seq_t values */
static inline void swap(seq_t *x, seq_t *y)
{
    seq_t t = *x; *x = *y; *y = t;
}

/* Generate the list of 12 divisors of 1701 and store it sorted.   */
static void build_divisors(void)
{
    if (div_cache_ready) return;

    /* powers of 3: 3^0 … 3^5 */
    seq_t p3[6];
    p3[0] = (seq_t)1;
    for (size_t i = 1; i <= 5; ++i)
        p3[i] = p3[i-1] * (seq_t)3;

    /* powers of 7: 7^0 … 7^1 */
    seq_t p7[2];
    p7[0] = (seq_t)1;
    p7[1] = p7[0] * (seq_t)7;

    /* build the 12 products */
    size_t idx = 0;
    for (size_t i = 0; i <= 5; ++i) {
        for (size_t j = 0; j <= 1; ++j) {
            div_cache[idx++] = p3[i] * p7[j];
        }
    }

    /* simple insertion sort (array size is tiny) */
    for (size_t i = 1; i < 12; ++i) {
        seq_t val = div_cache[i];
        size_t j = i;
        while (j > 0 && div_cache[j-1] > val) {
            div_cache[j] = div_cache[j-1];
            --j;
        }
        div_cache[j] = val;
    }

    div_cache_ready = true;
}

seq_t a(idx_t n)
{
    if (n < 1 || n > 12) {
        /* Out of bounds: OEIS sequences that are finite return 0. */
        return (seq_t)0;
    }

    if (!div_cache_ready)
        build_divisors();

    /* n is 1‑based, array is 0‑based */
    return div_cache[(size_t)(n - 1)];
}
