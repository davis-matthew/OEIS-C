/*
 * A156046  A triangle sequence made symmetrical by reverse coefficients:
 *          t0(n,m)=(2 + n! - m! - (n - m)! + 2 + PartitionsP[n] -
 *          PartitionsP[ m] - PartitionsP[n - m]);
 *          t(n,m)=(t0(n,m)+Reverse[t0(n,m)])/2.
 * https://oeis.org/A156046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   t0(n,m) = 4 + n! - m! - (n-m)! + P[n] - P[m] - P[n-m]
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS entry defines a(n) as the flattened triangle produced by t0(n,m). */

#include <stdlib.h>
#include <math.h>
#include <stddef.h>

/* idx_t and seq_t are supplied by the build (do not redefine them). */

static seq_t *fact_cache = NULL;   /* fact_cache[i] = i!  (0 <= i < fact_size) */
static idx_t   fact_size = 0;

static seq_t *part_cache = NULL;   /* part_cache[i] = P(i) (0 <= i < part_size) */
static idx_t   part_size = 0;

/* Ensure factorials for 0..n are precomputed. */
static void ensure_factorials(idx_t n)
{
    if (n + 1 <= fact_size) return;

    idx_t new_size = fact_size ? fact_size * 2 : 1;
    while (new_size <= n + 1)
        new_size *= 2;

    seq_t *new_cache = (seq_t *)realloc(fact_cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();
    fact_cache = new_cache;

    if (fact_size == 0) {
        fact_cache[0] = (seq_t)1;
        fact_size = 1;
    }

    for (idx_t i = fact_size; i <= n; ++i) {
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }
    fact_size = n + 1;
}

/* Ensure partition numbers for 0..n are precomputed using the generalized pentagonal recurrence. */
static void ensure_partitions(idx_t n)
{
    if (n + 1 <= part_size) return;

    idx_t new_size = part_size ? part_size * 2 : 1;
    while (new_size <= n + 1)
        new_size *= 2;

    seq_t *new_cache = (seq_t *)realloc(part_cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();
    part_cache = new_cache;

    if (part_size == 0) {
        part_cache[0] = (seq_t)1; /* P(0)=1 */
        part_size = 1;
    }

    /* Compute partitions from part_size up to n inclusive. */
    for (idx_t i = part_size; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            /* generalized pentagon numbers g1 and g2 */
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > i) break;
            /* sign pattern: + - + - ... (alternating every two terms) */
            if (k & 1) { /* k odd => plus */
                sum += part_cache[i - g1];
            } else {     /* k even => minus */
                sum -= part_cache[i - g1];
            }

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > i) continue;
            if (k & 1) {
                sum += part_cache[i - g2];
            } else {
                sum -= part_cache[i - g2];
            }
        }
        part_cache[i] = sum;
    }
    part_size = n + 1;
}

/* Main sequence access function */
seq_t a(idx_t idx)
{
    /* Find row r such that r*(r+1)/2 <= idx < (r+1)*(r+2)/2. */
    long double d = sqrtl((long double)1 + 8.0L * (long double)idx);
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);

    /* Correct possible rounding errors. */
    while (r * (r + 1) / 2 > idx) --r;
    while ((r + 1) * (r + 2) / 2 <= idx) ++r;

    idx_t m = idx - r * (r + 1) / 2;

    /* Ensure factorials and partitions up to r are available. */
    ensure_factorials(r);
    ensure_partitions(r);

    /* Compute t0(n,m). */
    seq_t result = (seq_t)4;
    result += fact_cache[r];
    result -= fact_cache[m];
    result -= fact_cache[r - m];
    result += part_cache[r];
    result -= part_cache[m];
    result -= part_cache[r - m];

    return result;
}
