/*
 * A237561  Number of (n+1)X(2+1) 0..2 arrays with the upper median plus the
 *          lower median of every 2X2 subblock differing from its horizontal
 *          and vertical neighbors by exactly one.
 * https://oeis.org/A237561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2) + 42*a(n-3) + 208*a(n-4) + 293*a(n-5) +
 *            900*a(n-6) - 12*a(n-7) - 857*a(n-8) - 1583*a(n-9) - 4726*a(n-10)
 *            + 4946*a(n-11) + 5993*a(n-12) + 3312*a(n-13) + 3822*a(n-14) -
 *            3319*a(n-15) - 540*a(n-16) - 32*a(n-17) for n ≥ 18. Initial
 *            values 1 ≤ n ≤ 17 taken from the OEIS table. The funct...
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for 1 ≤ n ≤ 17 */
    static const seq_t base[17] = {
        (seq_t)316ull,          /* a(1) */
        (seq_t)1536ull,         /* a(2) */
        (seq_t)7356ull,         /* a(3) */
        (seq_t)36168ull,        /* a(4) */
        (seq_t)175768ull,       /* a(5) */
        (seq_t)848720ull,       /* a(6) */
        (seq_t)4121060ull,      /* a(7) */
        (seq_t)20050460ull,     /* a(8) */
        (seq_t)97225972ull,     /* a(9) */
        (seq_t)471641072ull,    /* a(10) */
        (seq_t)2290359200ull,   /* a(11) */
        (seq_t)11118632740ull,  /* a(12) */
        (seq_t)53953373220ull,  /* a(13) */
        (seq_t)261887194692ull, /* a(14) */
        (seq_t)1271260706180ull,/* a(15) */
        (seq_t)6170274480252ull,/* a(16) */
        (seq_t)29948363314372ull/* a(17) */
    };

    /* Static memoisation cache: 1‑based indices stored at 0‑based positions */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity in elements */
    static idx_t cache_max = 0;    /* largest n with cache[n‑1] defined */

    /* Directly return short terms */
    if (n <= 17)
        return base[n - 1];

    /* Ensure cache can hold entries up to n */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)           /* allocation failed – return 0 as error */
            return (seq_t)0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialise base terms once and set cache_max = 17 */
    if (cache_max < 17) {
        for (idx_t i = 1; i <= 17; ++i)
            cache[i - 1] = base[i - 1];
        cache_max = 17;
    }

    /* Compute values for n ≥ 18 */
    idx_t start = (cache_max + 1 > (idx_t)18) ? (cache_max + 1) : (idx_t)18;
    for (idx_t i = start; i <= n; ++i) {
        seq_t val = 0;
        val += (seq_t)2   * cache[i - 3];  /* a(i-2)    */
        val += (seq_t)42  * cache[i - 4];  /* a(i-3)    */
        val += (seq_t)208 * cache[i - 5];  /* a(i-4)    */
        val += (seq_t)293 * cache[i - 6];  /* a(i-5)    */
        val += (seq_t)900 * cache[i - 7];  /* a(i-6)    */
        val -= (seq_t)12  * cache[i - 8];  /* a(i-7)    */
        val -= (seq_t)857 * cache[i - 9];  /* a(i-8)    */
        val -= (seq_t)1583 * cache[i -10];/* a(i-9)    */
        val -= (seq_t)4726 * cache[i -11];/* a(i-10)   */
        val += (seq_t)4946 * cache[i -12];/* a(i-11)   */
        val += (seq_t)5993 * cache[i -13];/* a(i-12)   */
        val += (seq_t)3312 * cache[i -14];/* a(i-13)   */
        val += (seq_t)3822 * cache[i -15];/* a(i-14)   */
        val -= (seq_t)3319 * cache[i -16];/* a(i-15)   */
        val -= (seq_t)540  * cache[i -17];/* a(i-16)   */
        val -= (seq_t)32   * cache[i -18];/* a(i-17)   */
        cache[i - 1] = val;
    }
    cache_max = n;

    return cache[n - 1];
}
