/*
 * A205625  Number of (n+1) X 8 0..2 arrays with every 2 X 2 subblock in a row
 *          having an equal number of equal diagonal or equal antidiagonal
 *          elements, adjacent rows differing in this number, and new values
 *          0..2 introduced in row major order.
 * https://oeis.org/A205625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1032*a(n-2) +144*a(n-3) -3000*a(n-4) -29184*a(n-5) *
 *            -260224*a(n-6) -294912*a(n-7) -12539904*a(n-8) *
 *            -218824704*a(n-10) -2415919104*a(n-12), for n>13 * Initial
 *            values (1 ≤ n ≤ 13): * 49154, 1284960, 42094188, 1324746888,
 *            43439036364, * 1367618567424, 44843430265344, 1412066783037696,
 *            * 46...
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 23,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

/* static cache for computed values a[0..n] */
static seq_t *cache = NULL;          /* cache[0] unused, cache[1].. for a(1..) */
static idx_t  cached_size = 0;       /* number of allocated entries */

/* cache of the 13 initial values (indices 1..13) */
static seq_t init_vals[14];
static bool init_done = false;

/* parse the 13 known terms once */
static void init_table(void)
{
    if (init_done) return;
    const char *s[14] = {
        "0",                      /* dummy for index 0 */
        "49154",
        "1284960",
        "42094188",
        "1324746888",
        "43439036364",
        "1367618567424",
        "44843430265344",
        "1412066783037696",
        "46294475000952672",
        "1457966169381319680",
        "47792560548918834432",
        "1505356985323702193664",
        "49339149731535883131648"
    };
    for (int i = 1; i <= 13; ++i) {
        init_vals[i] = seq_from_str(s[i]);
    }
    init_done = true;
}

/* extend cache to at least index n (inclusive) */
static void ensure_n(idx_t n)
{
    if (cached_size > n) return;          /* already computed up to n */
    if (!init_done) init_table();          /* parse constants once */

    /* allocate space for indices 0..n */
    seq_t *newcache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!newcache) abort();                /* out of memory */
    cache = newcache;

    /* compute missing values */
    for (idx_t i = cached_size; i <= n; ++i) {
        if (i == 0) {                      /* a(0) unused, set to 0 */
            cache[i] = (seq_t)0;
        } else if (i <= 13) {              /* use predetermined constants */
            cache[i] = init_vals[(int)i];
        } else {                           /* recurrence for n >= 14 */
            seq_t val = (seq_t)0;
            val += cache[i-2] * (seq_t)1032;
            val += cache[i-3] * (seq_t)144;
            val -= cache[i-4] * (seq_t)3000;
            val -= cache[i-5] * (seq_t)29184;
            val -= cache[i-6] * (seq_t)260224;
            val -= cache[i-7] * (seq_t)294912;
            val -= cache[i-8] * (seq_t)12539904;
            val -= cache[i-10] * (seq_t)218824704;
            val -= cache[i-12] * (seq_t)2415919104;
            cache[i] = val;
        }
    }
    cached_size = n + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not part of the OEIS offset */
    ensure_n(n);
    return cache[n];
}
