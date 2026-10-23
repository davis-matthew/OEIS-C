/*
 * A186461  Number of (n+1) X 9 binary arrays with every 2 X 2 subblock
 *          commuting with each of its horizontal and vertical 2 X 2 subblock
 *          neighbors.
 * https://oeis.org/A186461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + 2*a(n-4) - a(n-5) (for n > 14)
 *            Initial values (1 ≤ n ≤ 14): 359, 457, 846, 1493, 2285, 3677,
 *            5633, 9477, 13245, 20281, 27573, 43177, 62025, 98157
 * Range:     u8 none, u16 n <= 13, u32 n <= 39, u64 n <= 93, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* storage for computed terms (1‑based) */
static size_t cap   = 0;             /* allocated capacity */
static idx_t  max_n = 0;             /* largest n for which terms are known */

/* ensure that cache has room for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (n <= (idx_t)max_n) return;          /* already computed up to n */

    if (cap == 0) {                         /* first allocation: start at 16 */
        cap = (n > 16 ? n : 16);
    } else {
        while (cap < n)                   /* grow until enough */
            cap *= 2;
    }

    seq_t *tmp = (seq_t *)realloc(cache, (cap + 1) * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);         /* memory exhaustion */
    cache = tmp;
}

/* initialize the first 14 terms if not yet done */
static void init_first_terms(void)
{
    static const seq_t first[15] = {
        0,     /* unused, 1‑based indexing */
        359,   457,   846,  1493,  2285,  3677,
        5633,  9477, 13245, 20281, 27573, 43177,
        62025, 98157
    };

    if (max_n >= 14) return;            /* already initialized */

    ensure_capacity(14);                 /* make room for 1..14 */

    memcpy(&cache[1], first + 1, 14 * sizeof(seq_t));
    max_n = 14;
}

/* compute terms up to n as needed */
static void compute_up_to(idx_t n)
{
    init_first_terms();                  /* guarantees 1..14 exist */

    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* recurrence: a(i) = 2*a(i-1) - 2*a(i-3) + 2*a(i-4) - a(i-5) */
        seq_t val = cache[i-1] + cache[i-1];      /* 2 * a(i-1) */
        val -= cache[i-3] + cache[i-3];          /* subtract 2 * a(i-3) */
        val += cache[i-4] + cache[i-4];          /* add 2 * a(i-4) */
        val -= cache[i-5];                       /* subtract a(i-5) */
        cache[i] = val;
    }
    max_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of offset, undefined behaviour */
    ensure_capacity(n);
    compute_up_to(n);
    return cache[n];
}
