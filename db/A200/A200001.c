/*
 * A200001  Numbers that are the product of distinct decimal digits.
 * https://oeis.org/A200001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) is the sorted list of distinct products of subsets of the *
 *            decimal digits 0..9 (each digit may be used at most once). The
 *            first term * (n=1) is 0, the next is 1 (empty subset), etc. The
 *            sequence is finite * and contains 153 terms
 * Range:     u8 n <= 25, u16 n <= 125, u32 n <= 153, u64 n <= 153,
 *            u128 n <= 153, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/*  Comparator for qsort that works for any unsigned integer type or for
 *  arbitrary‑precision integer classes that implement the usual comparison
 *  operators. */
static int seq_cmp(const void *p1, const void *p2)
{
    const seq_t a = *(const seq_t *)p1;
    const seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/*  Cached sorted unique products.  They are built once on first call to a() */
static seq_t *seq_cache = NULL;  /* sorted array of unique products */
static size_t seq_len   = 0;     /* number of cached terms */

/*  Initialise the cache.  The number of subsets of the 10 digits is 2^10=1024.
 *  The maximum product is 9! = 362880 so the array never exceeds 1024
 *  elements, well within reasonable memory limits. */
static void init_cache(void)
{
    if (seq_cache != NULL) return;  /* already initialised */

    const size_t max_mask = 1U << 10;  /* 10 digits: 0..9 */
    /* Allocate space for all subset products first */
    size_t capacity = max_mask;
    seq_t *tmp = (seq_t *)malloc(capacity * sizeof(seq_t));
    if (!tmp) return; /* allocation failure – environment will handle */

    size_t count = 0;
    for (size_t mask = 0; mask < max_mask; ++mask) {
        seq_t prod = (seq_t)1;
        int has_zero = 0;
        for (int d = 0; d <= 9; ++d) {
            if (mask & (1U << d)) {
                if (d == 0) {   /* any subset with 0 gives product 0 */
                    prod = (seq_t)0;
                    has_zero = 1;
                    break;       /* no need to multiply further */
                }
                prod *= (seq_t)d;
            }
        }
        tmp[count++] = prod;
    }

    /* Sort the list */
    qsort(tmp, count, sizeof(seq_t), seq_cmp);

    /* Remove duplicates in‑place */
    size_t uniq = 0;
    for (size_t i = 0; i < count; ++i) {
        if (i == 0 || seq_cmp(&tmp[i - 1], &tmp[i]) != 0) {
            tmp[uniq++] = tmp[i];
        }
    }

    /* Reallocate to exact size */
    seq_t *real = (seq_t *)realloc(tmp, uniq * sizeof(seq_t));
    if (real) {
        seq_cache = real;
        seq_len   = uniq;
    } else {
        /* If realloc fails, keep the previous allocation to avoid leaks */
        seq_cache = tmp;
        seq_len   = uniq;
    }
}

/*  Return the nth term (1‑based) of the sequence.  If n is out of
 *  range or invalid, 0 is returned as a safe fallback. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1; return 0 */

    init_cache();

    if ((idx_t)(n - 1) >= seq_len)
        return (seq_t)0;   /* out of range – safe default */

    return seq_cache[(size_t)(n - 1)];
}
