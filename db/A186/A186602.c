/*
 * A186602  Number of (n+4) X 6 binary arrays with every 5 X 5 subblock
 *          commuting with each horizontal and vertical neighbor 5 X 5
 *          subblock.
 * https://oeis.org/A186602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n‑1) + a(n‑3) – a(n‑4) + a(n‑5) – a(n‑6) – a(n‑8) +
 *            a(n‑9) + a(n‑10) – a(n‑13) for n > 20 with a(1)=6187, a(2)=543,
 *            a(3)=916, a(4)=3455, a(5)=10882, a(6)=23977, a(7)=38625,
 *            a(8)=49309, a(9)=52849, a(10)=53957, a(11)=54873, a(12)=55797,
 *            a(13)=56857, a(14)=59338, a(15)=67619, a(16)=89028,...
 * Range:     u8 none, u16 n <= 14, u32 n <= 65, u64 n <= 171, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* 1‑based array of computed terms */
static idx_t  cache_size = 0;        /* allocated length (edges are 0 … size‑1) */
static idx_t  max_computed = 0;      /* highest sorted index computed */

/* 1‑based lookup table for the first 20 values (index 0 unused) */
static const seq_t init_val[21] = {
    0ULL,          /* 0 - unused */
    6187ULL,       /* 1 */
    543ULL,        /* 2 */
    916ULL,        /* 3 */
    3455ULL,       /* 4 */
    10882ULL,      /* 5 */
    23977ULL,      /* 6 */
    38625ULL,      /* 7 */
    49309ULL,      /* 8 */
    52849ULL,      /* 9 */
    53957ULL,      /*10 */
    54873ULL,      /*11 */
    55797ULL,      /*12 */
    56857ULL,      /*13 */
    59338ULL,      /*14 */
    67619ULL,      /*15 */
    89028ULL,      /*16 */
    127253ULL,     /*17 */
    177594ULL,     /*18 */
    232896ULL,     /*19 */
    295106ULL      /*20 */
};

/* Ensure the cache can store indices up to needed (inclusive) */
static void ensure_capacity(idx_t needed)
{
    if (needed + 1 <= cache_size)
        return;

    size_t new_size = (size_t)(needed + 1);
    if (cache == NULL)
        cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    else
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

    /* On allocation failure, leave cache untouched – sequence may not
       be usable but behaviour for invalid memory is undefined anyway. */
    cache_size = new_size;
}

/* Return the n‑th term of the sequence.  n == 0 has no value, return 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but defined as 0 for safety */

    ensure_capacity(n);

    /* If requested value already computed, just return it. */
    if (n <= max_computed)
        return cache[n];

    /* Compute missing values up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        if (i <= 20) {
            cache[i] = init_val[i];
        } else {
            seq_t v = cache[i - 1]
                   + cache[i - 3]
                   - cache[i - 4]
                   + cache[i - 5]
                   - cache[i - 6]
                   - cache[i - 8]
                   + cache[i - 9]
                   + cache[i -10]
                   - cache[i -13];
            cache[i] = v;
        }
    }
    max_computed = n;
    return cache[n];
}
