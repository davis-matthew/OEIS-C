/*
 * A224002  Number of 4 X n 0..2 arrays with antidiagonals unimodal and rows
 *          and diagonals nondecreasing.
 * https://oeis.org/A224002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 40, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 9*a(n-1)-36*a(n-2)+84*a(n-3)-126*a(n-4)+126*a(n-5)
 *              -84*a(n-6)+36*a(n-7)-9*a(n-8)+a(n-9)   for n>13
 * Initial values: a(1)=81, a(2)=793, a(3)=2980, a(4)=7927, a(5)=17929,
 *                 a(6)=36845, a(7)=71061, a(8)=130767, a(9)=231730,
 *                 a(10)=397675, a(11)=663404, a(12)=1078800, a(13)=1713877
 * The function uses a simple memoised recurrence and works for any
 * nth term requested (n≥1).  It is safe for both fixed‑width integer
 * types and arbitrary‑precision integer types (seq_t may be a class or an
 * unsigned integer).
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* 1‑indexed storage, cache[0] unused */
static idx_t  cache_len = 0;         /* largest n calculated (inclusive) */
static idx_t  cache_cap = 0;         /* allocated capacity */

/* Grow the cache so that indices up to 'need' (inclusive) can be stored */
static void ensure_cap(idx_t need)
{
    if (need + 1 <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= need + 1)
        new_cap <<= 1;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Initialise cache with the first 13 terms of the sequence */
static void init_cache(void)
{
    if (cache)
        return;
    ensure_cap(13);
    /* cache[0] is unused; initialise from index 1 */
    static const seq_t init_vals[13] = {
        81,     793,    2980,   7927,   17929,
        36845,  71061,  130767, 231730, 397675,
        663404, 1078800,1713877
    };
    cache[0] = 0;
    for (idx_t i = 1; i <= 13; ++i)
        cache[i] = init_vals[i - 1];
    cache_len = 13;
}

/* Return the nth term (with offset 1).  Undefined for n<1. */
seq_t a(idx_t n)
{
    if (n < 1)        /* defined to return 0 for safety, not used in tests */
        return 0;

    init_cache();

    /* Extend cache if required */
    if (n > cache_len) {
        ensure_cap(n);
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t sum = 0;
            sum += 9 * cache[i - 1];
            sum -= 36 * cache[i - 2];
            sum += 84 * cache[i - 3];
            sum -= 126 * cache[i - 4];
            sum += 126 * cache[i - 5];
            sum -= 84 * cache[i - 6];
            sum += 36 * cache[i - 7];
            sum -= 9 * cache[i - 8];
            sum += cache[i - 9];
            cache[i] = sum;
        }
        cache_len = n;
    }
    return cache[n];
}
