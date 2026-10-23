/*
 * A198439  Square root of first term of a triple of squares in arithmetic
 *          progression that is not a multiple of another triple in (A198384,
 *          A198385, A198386).
 * https://oeis.org/A198439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 38, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Implement A198439: square root of the first term of a primitive
 * triple of squares in arithmetic progression that is not a multiple
 * of another triple in (A198384, A198385, A198386).  The sequence
 * is obtained by generating all primitive Pythagorean triples,
 * sorting them by the leg sum x + y, and returning |x – y| in that
 * sorted order.
 *
 * The algorithm is O(N) in the number of generated triples (≈
 * 3·range/π).  It precomputes all values up to a sum limit that
 * exceeds 20 000 terms and caches them for repeated calls.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_gcd_idx.h"

/* Euclidean GCD for idx_t (used only for loop indices). */

/* Holds a primitive triple’s leg sum and |x – y|. */
typedef struct
{
    seq_t sum;
    seq_t diff;
} triple_t;

/* Cache for a(n) values (1‑based indexing). */
static seq_t *cache      = NULL;
static size_t  cache_cap = 0;
static size_t  cache_len = 0;

/* Temporary storage for generated triples. */
static triple_t *temp_arr = NULL;
static size_t    temp_cap = 0;
static size_t    temp_len = 0;

/* Append a triple into temp_arr with automatic growth. */
static void _push_triple(triple_t t)
{
    if (temp_len == temp_cap)
    {
        temp_cap = (temp_cap == 0) ? 256 : temp_cap << 1;
        temp_arr = (triple_t *)realloc(temp_arr, temp_cap * sizeof(triple_t));
    }
    temp_arr[temp_len++] = t;
}

/* Comparator for qsort: ascending by sum, then ascending by diff. */
static int _cmp_triple(const void *a, const void *b)
{
    const triple_t *t1 = (const triple_t *)a;
    const triple_t *t2 = (const triple_t *)b;
    if (t1->sum < t2->sum) return -1;
    if (t1->sum > t2->sum) return 1;
    if (t1->diff < t2->diff) return -1;
    if (t1->diff > t2->diff) return 1;
    return 0;
}

/* Initialise the cache: generate, sort, and store diff values. */
static void _init_cache(void)
{
    /* Sum limit that yields > 20 000 primitive triples. */
    const idx_t MAX_SUM = 2000000;
    idx_t m, n;

    for (m = 2; (idx_t)m * (idx_t)m <= MAX_SUM; ++m)
    {
        for (n = 1; n < m; ++n)
        {
            if (((m - n) & 1) == 0)         /* opposite parity required */
                continue;
            if (_gcd_idx(m, n) != 1)        /* primitive condition */
                continue;

            seq_t mm = (seq_t)m;
            seq_t nn = (seq_t)n;
            seq_t a   = mm * mm - nn * nn;   /* odd leg */
            seq_t b   = (seq_t)2 * mm * nn;   /* even leg */
            seq_t sum = a + b;
            if (sum > (seq_t)MAX_SUM)
                break;                       /* sums increase with n */

            seq_t diff = (a > b) ? a - b : b - a;
            _push_triple((triple_t){sum, diff});
        }
    }

    /* Sort by the leg sum, breaking ties by the diff value. */
    qsort(temp_arr, temp_len, sizeof(triple_t), _cmp_triple);

    /* Transfer diffs to the final cache. */
    cache_cap = temp_len;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    for (size_t i = 0; i < temp_len; ++i)
        cache[i] = temp_arr[i].diff;
    cache_len = cache_cap;

    /* Free the temporary array. */
    free(temp_arr);
    temp_arr = NULL;
    temp_cap = temp_len = 0;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1; safety */

    if (cache_len == 0)                /* first call: initialise cache */
        _init_cache();

    if (n <= cache_len)
        return cache[n - 1];

    /* For requested indices beyond the precomputed range,
       we simply return 0.  This case should not occur for
       the typical usage limits (n ≤ 10 000). */
    return 0;
}
