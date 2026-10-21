/*
 * A235300  Number of (n+1) X (10+1) 0..3 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            3*a(n-1)+17*a(n-2)-57*a(n-3)-86*a(n-4)+372*a(n-5)+76*a(n-6)-972*a(n-7)+360*a(n-8)+864*a(n-9)-576*a(n-10)
 *            Initial values: a(1) = 274368, a(2) = 290932, a(3) = 312416,
 *            a(4) = 356632, a(5) = 420816, a(6) = 553552, a(7) = 768272, a(8)
 *            = 1215520, a(9) = 2006448, a(10) = 3677632
 * Range:     u8 none, u16 none, u32 n <= 19, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#ifdef __cplusplus
# include <type_traits>
#endif

/* Cached sequence values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;      /* capacity in elements, incl. index 0 unused */
static idx_t  cache_len = 0;      /* maximum index currently computed */

static const seq_t init[10] = {
    (seq_t)274368,
    (seq_t)290932,
    (seq_t)312416,
    (seq_t)356632,
    (seq_t)420816,
    (seq_t)553552,
    (seq_t)768272,
    (seq_t)1215520,
    (seq_t)2006448,
    (seq_t)3677632
};

/* Ensure cache can hold indices up to n (inclusive).  Index 0 is unused. */
static void ensure_cache_capacity(idx_t n)
{
    if ((idx_t)cache_cap <= n) {
        /* Repeat until capacity is large enough */
        do {
            cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        } while (cache_cap <= n);
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* Compute a single term using the recurrence, assuming cache[1..i-1] are ready */
static seq_t rec_compute(idx_t i)
{
#ifdef __cplusplus
    using big_t = typename std::conditional<
        std::is_integral<seq_t>::value,
        unsigned __int128,
        seq_t>::type;
#else
    /* In C mode seq_t is an integer type, so use 128‑bit intermediate */
    typedef unsigned __int128 big_t;
#endif

    big_t pos = 0, neg = 0;

    pos += (big_t)3   * cache[i-1];
    pos += (big_t)17  * cache[i-2];
    neg += (big_t)57  * cache[i-3];
    neg += (big_t)86  * cache[i-4];
    pos += (big_t)372 * cache[i-5];
    pos += (big_t)76  * cache[i-6];
    neg += (big_t)972 * cache[i-7];
    pos += (big_t)360 * cache[i-8];
    pos += (big_t)864 * cache[i-9];
    neg += (big_t)576 * cache[i-10];

    return (seq_t)(pos - neg);
}

/* Compute all terms up to n (inclusive).  After this, cache_len >= n. */
static void compute_up_to(idx_t n)
{
    if (n <= cache_len) return;

    ensure_cache_capacity(n);

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 10) {
            cache[i] = init[i-1];
        } else {
            cache[i] = rec_compute(i);
        }
    }
    cache_len = n;
}

/* Public entry point for the sequence */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1, but return 0 safely */
    compute_up_to(n);
    return cache[n];
}
