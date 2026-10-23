/*
 * A186615  Number of (n+5)X10 binary arrays with every 6X6 subblock commuting
 *          with each horizontal and vertical neighbor 6X6 subblock.
 * https://oeis.org/A186615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence
 *            a(n)=a(n‑1)+a(n‑4)‑a(n‑5)+a(n‑6)‑a(n‑7)‑a(n‑10)+a(n‑11)+a(n‑12)‑a(n‑16)
 *            for n>22 Initial values (n=1..22): 3764191, 137138, 172192,
 *            439891, 1630171, 3608091, 4710363, 4814107, 4823775, 4832547,
 *            4841327, 4850135, 4859207, 4871003, 4898423, 5002339, 5546319,
 *            7716983, 12433160,...
 * Range:     u8 none, u16 none, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[22] = {
    3764191ULL, 137138ULL, 172192ULL, 439891ULL, 1630171ULL,
    3608091ULL, 4710363ULL, 4814107ULL, 4823775ULL, 4832547ULL,
    4841327ULL, 4850135ULL, 4859207ULL, 4871003ULL, 4898423ULL,
    5002339ULL, 5546319ULL, 7716983ULL, 12433160ULL, 17255805ULL,
    22103742ULL, 27036947ULL
};

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                 /* undefined for n<1 */

    /* 1‑based cache */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static idx_t   len   = 0;

    /* ----- allocate / grow cache as needed ----- */
    if (!cache) {
        cap   = 64;                         /* initial capacity (index 0 unused) */
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) return (seq_t)0;
    }
    if ((size_t)n + 1 > cap) {               /* need room for index n */
        size_t new_cap = cap;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cap   = new_cap;
    }

    /* ----- initialise first 22 terms if not done yet ----- */
    if (len < 22) {
        size_t start = (size_t)(len + 1);
        for (size_t i = start; i <= 22; ++i) {
            cache[i] = init_vals[i - 1];
        }
        len = 22;
    }

    /* ----- return if already computed ----- */
    if (n <= len) return cache[n];

    /* ----- compute terms using recurrence for n > 22 ----- */
    for (idx_t i = len + 1; i <= n; ++i) {
        /* all referenced indices are valid because i >= 23 in this loop */
        seq_t v = cache[i - 1] + cache[i - 4] - cache[i - 5]
                 + cache[i - 6] - cache[i - 7]
                 - cache[i -10] + cache[i -11]
                 + cache[i -12] - cache[i -16];
        cache[i] = v;
    }
    len = n;
    return cache[n];
}
