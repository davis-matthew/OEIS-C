/*
 * A208390  Number of nX6 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to more than two of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A208390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: A208390 is defined by the linear recurrence
 *
 *   a(n) = 649*a(n-1) - 8088*a(n-2) + 32724*a(n-3) - 69156*a(n-4)
 *          + 80352*a(n-5) - 46656*a(n-6)        for n > 7
 *
 * Initial values:
 *   a(1)=122, a(2)=76806, a(3)=48890520, a(4)=31112194860,
 *   a(5)=19798894881900, a(6)=12599441934740388, a(7)=8017919094401985204
 *
 * The function below memoises the values so it can be called in any order.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* The types seq_t and idx_t are provided by the build configuration. */
static seq_t *cache = NULL;      /* array holding computed terms (index 0 unused) */
static idx_t  cache_size = 0;    /* current capacity of 'cache' (number of slots) */
static idx_t  computed  = 0;     /* greatest index that has already been computed */

/* Helper: ensure the cache can hold index 'idx' (0‑based). */
static void
ensure_cache_size(idx_t idx)
{
    if (idx < cache_size)
        return;
    idx_t new_cap = cache_size ? cache_size * 2 : 16;
    if (new_cap <= idx)
        new_cap = idx + 1;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!tmp)
        return; /* in a real program we would handle the error */
    /* Zero‑initialize new entries */
    if (new_cap > cache_size)
        memset(tmp + cache_size, 0, (size_t)(new_cap - cache_size) * sizeof(seq_t));
    cache = tmp;
    cache_size = new_cap;
}

/* The main function as requested */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* behavior for n<1 is undefined; return 0 */

    ensure_cache_size(n);

    /* If already computed, return it immediately */
    if (n <= computed)
        return cache[n];

    /* Initial values for n = 1..7 */
    static const seq_t init[7] = {
        (seq_t)122ULL,
        (seq_t)76806ULL,
        (seq_t)48890520ULL,
        (seq_t)31112194860ULL,
        (seq_t)19798894881900ULL,
        (seq_t)12599441934740388ULL,
        (seq_t)8017919094401985204ULL
    };

    /* Compute all required terms up to 'n' */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 7) {
            cache[i] = init[i - 1];
        } else {
            seq_t val  = cache[i - 1] * (seq_t)649;
            val       -= cache[i - 2] * (seq_t)8088;
            val       += cache[i - 3] * (seq_t)32724;
            val       -= cache[i - 4] * (seq_t)69156;
            val       += cache[i - 5] * (seq_t)80352;
            val       -= cache[i - 6] * (seq_t)46656;
            cache[i] = val;
        }
    }
    computed = n;
    return cache[n];
}
