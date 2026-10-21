/*
 * A223483  Rolling icosahedron face footprints: number of 5Xn 0..19 arrays
 *          starting with 0 where 0..19 label faces of an icosahedron and
 *          every array movement to a horizontal or antidiagonal neighbor
 *          moves across an icosahedral edge.
 * https://oeis.org/A223483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 33*a(n-1) -86*a(n-2) -1564*a(n-3) +7040*a(n-4)
 *            -6480*a(n-5) -5088*a(n-6) +5824*a(n-7) -512*a(n-8) for n>13
 *            Initial values: a(1)=160000 a(2)=19683 a(3)=455625 a(4)=10546875
 *            a(5)=288507825 a(6)=7939566675 a(7)=223894186425
 *            a(8)=6318290752875 a(9)=178826926010625 a(10)=5061686863486275
 *            a(11)=...
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/seq_from_str.h"

static void ensure_cache(idx_t n, seq_t **ptr, size_t *len, size_t *cap)
{
    if (n <= *cap) return;
    size_t newcap = (*cap == 0) ? 16 : *cap;
    while (newcap < (size_t)n)
        newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(*ptr, newcap * sizeof(seq_t));
    if (!tmp) abort();
    *ptr = tmp;
    *cap = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* behaviour below offset not required */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of entries stored (1‑based) */
    static size_t cache_cap = 0;   /* allocated capacity */

    ensure_cache(n, &cache, &cache_len, &cache_cap);

    /* string representations of the first 14 terms */
    static const char *init_str[14] = {
        "160000",
        "19683",
        "455625",
        "10546875",
        "288507825",
        "7939566675",
        "223894186425",
        "6318290752875",
        "178826926010625",
        "5061686863486275",
        "143298073856716425",
        "4056837300096090075",
        "114852424247153593425",
        "3251568434305627847475"
    };

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 14) {
            /* convert decimal string to seq_t */
            val = seq_from_str(init_str[i - 1]);
        } else {
            /* compute using recurrence */
            seq_t pos = cache[i - 2] * (seq_t)33; /* a(n-1) * 33 */
            pos += cache[i - 5] * (seq_t)7040;    /* a(n-4) * 7040 */
            pos += cache[i - 8] * (seq_t)5824;    /* a(n-7) * 5824 */

            seq_t neg = cache[i - 3] * (seq_t)86;     /* a(n-2) * 86 */
            neg += cache[i - 4] * (seq_t)1564;        /* a(n-3) * 1564 */
            neg += cache[i - 6] * (seq_t)6480;        /* a(n-5) * 6480 */
            neg += cache[i - 7] * (seq_t)5088;        /* a(n-6) * 5088 */
            neg += cache[i - 9] * (seq_t)512;         /* a(n-8) * 512 */

            val = pos - neg; /* unsigned subtraction; legal since pos>=neg */
        }
        cache[i - 1] = val;
        cache_len = (size_t)i;
    }

    return cache[n - 1];
}
