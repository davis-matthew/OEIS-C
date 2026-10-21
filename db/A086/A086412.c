/*
 * A086412  Number of distinct prime factors of 3-smooth numbers.
 * https://oeis.org/A086412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime factors of the n-th 3‑smooth
 *            number. a(n) = 0 if the number is 1, a(n) = 1 if it is a power
 *            of 2 or a power of 3, a(n) = 2 otherwise
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The 3‑smooth numbers are generated in increasing order using two‑pointer
   technique.  We store for each number the exponents of 2 and 3 and the
   logarithm of its value (base e).  The logarithm is used to compare the
   two next candidates without computing the huge product explicitly.  Only
   values up to the largest requested index are generated; thus the
   algorithm is O(n) in total and works for all n >= 1. */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

typedef unsigned int uint32_t;   /* makes sure idx_t and seq_t are known types */

/* The caller supplies these types; we just use them. */
extern uint64_t *smooth;       /* not used outside this file */

/* Structure holding the exponents of 2 and 3 for a 3‑smooth number
   and its logarithm for fast comparison. */
struct smooth_entry {
    unsigned int e2;
    unsigned int e3;
    long double logv;   /* e2*ln(2)+e3*ln(3) */
};

static struct smooth_entry *smooth_tbl = NULL;
static size_t smooth_sz = 0;      /* number of elements currently stored   */
static size_t smooth_cap = 0;     /* allocated capacity                   */
static size_t idx2 = 0, idx3 = 0; /* indices for next multiple of 2 and 3 */

static const long double LN2 = 0.6931471805599453094172321214581765680755L;
static const long double LN3 = 1.0986122886681096913952452369225257044631L;

/* Grow the table to contain at least n elements (n is 1‑based).  The table
   is never shrunk. */
static void ensure_smooth(size_t n)                     /* n >= 1 */
{
    if (smooth_sz >= n)
        return;

    /* enlarge capacity if necessary */
    if (smooth_cap == 0)
        smooth_cap = 1;
    while (smooth_cap < n)
        smooth_cap <<= 1;

    struct smooth_entry *newptr = (struct smooth_entry *)realloc(smooth_tbl, smooth_cap * sizeof(struct smooth_entry));
    if (!newptr) abort();   /* allocation failed */
    smooth_tbl = newptr;

    /* initialise the first entry */
    if (smooth_sz == 0) {
        smooth_tbl[0].e2 = 0;
        smooth_tbl[0].e3 = 0;
        smooth_tbl[0].logv = 0.0L;
        smooth_sz = 1;
        idx2 = idx3 = 0;
    }

    /* generate missing entries */
    for (size_t k = smooth_sz; k < n; ++k) {
        unsigned int e2_c1 = smooth_tbl[idx2].e2 + 1;
        unsigned int e3_c1 = smooth_tbl[idx2].e3;
        unsigned int e2_c2 = smooth_tbl[idx3].e2;
        unsigned int e3_c2 = smooth_tbl[idx3].e3 + 1;

        /* If the two candidates are equal, record it only once
           and advance both indices.  Equality condition follows from exact
           equality of the underlying integers. */
        if (e2_c1 == e2_c2 && e3_c1 == e3_c2) {
            smooth_tbl[k].e2 = e2_c1;
            smooth_tbl[k].e3 = e3_c1;
            smooth_tbl[k].logv = smooth_tbl[idx2].logv + LN2;  /* or + LN3 */
            idx2++;
            idx3++;
            continue;
        }

        /* compare the logarithms of the two candidates */
        long double log1 = smooth_tbl[idx2].logv + LN2;
        long double log2 = smooth_tbl[idx3].logv + LN3;

        if (log1 < log2) {
            smooth_tbl[k].e2 = e2_c1;
            smooth_tbl[k].e3 = e3_c1;
            smooth_tbl[k].logv = log1;
            idx2++;
        } else {
            smooth_tbl[k].e2 = e2_c2;
            smooth_tbl[k].e3 = e3_c2;
            smooth_tbl[k].logv = log2;
            idx3++;
        }
    }

    smooth_sz = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset handling – unused */

    ensure_smooth((size_t)n);          /* ensure we have n entries */

    struct smooth_entry *e = &smooth_tbl[(size_t)n - 1];
    int cnt = 0;
    if (e->e2 > 0) cnt++;
    if (e->e3 > 0) cnt++;
    return (seq_t)cnt;
}
