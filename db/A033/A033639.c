/*
 * A033639  Base-2 digital convolution sequence.
 * https://oeis.org/A033639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = Σ_{i: bit i of n set} a(n - i - 1), a(0) = 1
 * Range:     u8 n <= 22, u16 n <= 42, u32 n <= 82, u64 n <= 154,
 *            u128 n <= 294, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Base-2 digital convolution sequence (OEIS A033639) */

#include <stdlib.h>
#include <stdint.h>

static seq_t* cache = NULL;   /* memoization array */
static size_t cap   = 0;      /* current capacity of cache */

/* Ensure cache is large enough to access index n */
static void ensure_capacity(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= cap) return;

    size_t new_cap = cap ? cap << 1 : 1;
    while (new_cap < need) new_cap <<= 1;

    seq_t* new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);           /* allocation failed */

    /* Initialize new elements to 0 */
    if (cache) {
        for (size_t i = 0; i < cap; ++i) new_cache[i] = cache[i];
        free(cache);
    }
    for (size_t i = cap; i < new_cap; ++i) new_cache[i] = 0;

    cache = new_cache;
    cap   = new_cap;
}

/* Recursive computation with memoization */
seq_t a(idx_t n)
{
    ensure_capacity(n);

    /* Return cached value if already computed */
    if (cache[n] != 0) return cache[n];

    /* Base case */
    if (n == 0) {
        cache[0] = 1;
        return 1;
    }

    seq_t sum = 0;
    idx_t temp = n;
    idx_t shift = 0;
    while (temp) {
        if (temp & 1) {
            /* Recursive call for the referenced term */
            sum += a(n - shift - 1);
        }
        shift++;
        temp >>= 1;
    }

    cache[n] = sum;
    return sum;
}
