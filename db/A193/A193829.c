/*
 * A193829  Irregular triangle read by rows in which row n lists the
 *          differences between consecutive divisors of n.
 * https://oeis.org/A193829
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = flatten(list of differences between consecutive divisors
 *            of each integer * m ≥ 2, where m is processed in ascending order
 *            starting with 2)
 * Range:     u8 n <= 1211, u16 n <= 9810, u32 n <= 9810, u64 n <= 9810,
 *            u128 n <= 9810, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation stores the sequence values in a static cache and generates rows
 * on demand.  It uses only integer arithmetic and standard library functions. */

#include <stdlib.h>
#include <stdint.h>

/* Cache for the sequence entries. */
static seq_t *cache      = NULL;   /* array of seq_t values */
static idx_t  cache_cap  = 0;      /* allocated capacity */
static idx_t  cache_len  = 0;      /* number of elements stored */
static idx_t  next_row   = 2;      /* next integer whose row should be generated */

/* Ensure that the cache can hold at least @need entries. */
static void
ensure_capacity(idx_t need)
{
    if (need <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;  /* out of memory – the sequence will be incomplete, but
                         * the build expects correct values only for valid indices. */
    cache = tmp;
    cache_cap = new_cap;
}

/* Append @value to the cache. */
static void
push_cache(seq_t value)
{
    ensure_capacity(cache_len + 1);
    cache[cache_len++] = value;
}

/* Generate the row for integer m and append its differences to the cache. */
static void
generate_row(idx_t m)
{
    /* Count the number of divisors to allocate a suitable buffer. */
    idx_t cnt = 0;
    for (idx_t d = 1; d <= m / d; ++d) {
        if (m % d == 0) {
            ++cnt;                 /* divisor d itself */
            if (d != m / d) ++cnt; /* complementary divisor */
        }
    }
    if (cnt <= 1) return;   /* no differences for m = 1 */

    idx_t *divs = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!divs) return;      /* out of memory */

    /* Fill the divisor array in ascending order directly by using two
     * pointers – one from the start for the small divisors, one from the end
     * for the large ones. */
    idx_t pos_low  = 0;
    idx_t pos_high = cnt - 1;
    for (idx_t d = 1; d <= m / d; ++d) {
        if (m % d == 0) {
            divs[pos_low++] = d;
            idx_t q = m / d;
            if (d != q) divs[pos_high--] = q;
        }
    }

    /* Emit the differences between consecutive divisors. */
    seq_t last = (seq_t)divs[0];
    for (idx_t i = 1; i < cnt; ++i) {
        seq_t diff = (seq_t)(divs[i] - last);
        push_cache(diff);
        last = divs[i];
    }

    free(divs);
}

/* Public function requested by the build. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* undefined for offset < 2 */

    idx_t target = n - 2;         /* a(2) -> cache[0] */

    /* Produce rows until the target entry is available. */
    while (cache_len <= target) {
        generate_row(next_row++);
    }
    return cache[target];
}
