/*
 * A192115  Monotonic ordering of nonnegative differences 5^i-2^j, for
 *          40>=i>=0, j>=0.
 * https://oeis.org/A192115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth smallest non‑negative integer of the form 5^i −
 *            2^j with 0 ≤ i ≤ 40 and j ≥ 0. The sequence is the monotonic
 *            ordering of all such differences, without duplication
 * Range:     u8 n <= 12, u16 n <= 40, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation detail:  All differences are pre‑computed once into a
   static sorted array.  The array contains every value that fits into
   5^i for i ≤ 40 and j large enough that the difference stays
   non‑negative.  Because 5^i grows faster than 2^j, values for larger
   i are larger than all values for smaller i; nevertheless a final
   sort guarantees correctness for any seq_t type. */

#include <stdlib.h>

static int seqcmp(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    return (*x < *y) ? -1 : (*x > *y) ? 1 : 0;
}

/* static cache of precomputed values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void generate_cache(void)
{
    if (cache != NULL) return; /* already generated */

    /* Rough upper bound: roughly 2000 values. */
    cache_cap = 2000;
    cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    if (cache == NULL) return; /* allocation failed – should never happen */

    /* Compute 5^i for i = 0..40, stop on overflow of seq_t */
    seq_t pow5[41];
    pow5[0] = 1;
    size_t max_i = 0;
    for (idx_t i = 1; i <= 40; ++i)
    {
        pow5[i] = pow5[i-1] * (seq_t)5;
        /* Detect unsigned overflow: the value becomes smaller after wrap‑around. */
        if (pow5[i] < pow5[i-1]) break;
        max_i = i;
    }

    /* For each valid i, generate all possible differences 5^i - 2^j (>=0). */
    for (idx_t i = 0; i <= max_i; ++i)
    {
        seq_t val = pow5[i];

        /* Find largest j such that 2^j <= val.  We use shifting, safe because
           j is at most ~90 for the largest val that fits into seq_t. */
        idx_t maxj = 0;
        while (( (seq_t)1 << maxj ) <= val) ++maxj;
        /* Back one step to get the last valid j. */
        if (maxj > 0) --maxj;

        /* Append differences in ascending order (j descending). */
        for (int jj = (int)maxj; jj >= 0; --jj)
        {
            seq_t diff = val - ((seq_t)1 << jj);

            if (cache_len >= cache_cap)
            {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache,
                                         cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = diff;
        }
    }

    /* Sort the array and remove (unlikely) duplicates. */
    qsort(cache, cache_len, sizeof(seq_t), seqcmp);

    size_t uniq = 0;
    for (size_t k = 0; k < cache_len; ++k)
    {
        if (k == 0 || cache[k] != cache[uniq-1])
            cache[uniq++] = cache[k];
    }
    cache_len = uniq;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of bounds – no official value */

    generate_cache();

    if ((idx_t)n <= cache_len)
        return cache[n - 1];
    return 0; /* undefined for n > cache_len (sequence is finite) */
}
