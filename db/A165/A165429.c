/*
 * A165429  a(1) = 1; a(n) = (Sum_{i=1..n-1} a(i))^n for n >= 2.
 * https://oeis.org/A165429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∑_{i=1}^{n-1} a(i))^n, with a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow, resize
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/resize.h"

/* Memoisation storage */
static seq_t *seq_cache   = NULL;   /* a(n) values (index n-1)          */
static seq_t *sum_prefix  = NULL;   /* prefix sums Σ_{i=1}^{k} a(i)      */
static size_t cache_cap   = 0;      /* allocated capacity                */
static idx_t  computed    = 0;      /* highest n already computed        */

/* Ensure buffers can hold at least (n) terms (index 0..n-1). */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < (size_t)n) new_cap <<= 1;

    seq_cache   = resize(seq_cache, new_cap);
    sum_prefix  = resize(sum_prefix, new_cap);
    cache_cap   = new_cap;
}

/* Sequence computation */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety guard – not used by OEIS */

    /* compute any missing terms up to n */
    if (n > computed) {
        ensure_capacity(n);
        for (idx_t k = computed + 1; k <= n; ++k) {
            seq_t value;
            if (k == 1) {
                value = 1;                     /* a(1) = 1 */
            } else {
                seq_t base = sum_prefix[k - 2];  /* Σ_{i=1}^{k-1} a(i) */
                value = big_pow(base, (seq_t)k);
            }
            seq_cache[k - 1] = value;
            if (k == 1) {
                sum_prefix[0] = value;
            } else {
                sum_prefix[k - 1] = sum_prefix[k - 2] + value;
            }
        }
        computed = n;
    }
    return seq_cache[n - 1];
}
