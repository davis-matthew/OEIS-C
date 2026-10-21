/*
 * A099467  a(1) = a(2) = 1; for n > 2, a(n) is the smallest number > a(n-1)
 *          which is not the sum of 2 consecutive elements of the sequence.
 * https://oeis.org/A099467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = a(2) = 1; for n > 2, a(n) = smallest integer > a(n-1)
 *            that is not equal to a(i)+a(i+1) for any i < n-1
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *terms   = NULL;   /* stored terms a(1), a(2), ... */
static size_t  terms_cap = 0;
static size_t  terms_len = 0;   /* number of terms already computed */

static seq_t *sums    = NULL;   /* a(i)+a(i+1) for i = 1 .. terms_len-1, in increasing order */
static size_t  sums_cap = 0;
static size_t  sums_len = 0;
static size_t  sums_idx = 0;    /* index into sums[] while searching for a new term */

/* Return a(n), where n >= 1. */
seq_t a(idx_t n)
{
    /* On first call, initialise storage with the first two terms. */
    if (!terms) {
        terms_cap = 16;
        terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
        sums_cap = 16;
        sums = (seq_t*)malloc(sums_cap * sizeof(seq_t));

        terms[0] = (seq_t)1;   /* a(1) */
        terms[1] = (seq_t)1;   /* a(2) */
        terms_len = 2;

        sums[0] = terms[0] + terms[1];  /* a(1)+a(2) */
        sums_len = 1;
    }

    /* If the requested index is already known, return it. */
    if (n <= terms_len)
        return terms[n-1];

    /* Generate terms up to n. */
    for (idx_t i = terms_len; i < n; ++i) {
        /* Candidate must be strictly greater than the previous term. */
        seq_t candidate = terms[terms_len-1] + 1;

        /* Skip values that appear in the sums array. */
        while (1) {
            while (sums_idx < sums_len && sums[sums_idx] < candidate)
                ++sums_idx;
            if (sums_idx < sums_len && sums[sums_idx] == candidate) {
                ++candidate;      /* forbidden, try next integer */
                continue;
            }
            break;                /* candidate is allowed */
        }

        /* Ensure capacity for the new term. */
        if (terms_len >= terms_cap) {
            terms_cap = terms_cap * 2;
            terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
        }
        terms[terms_len++] = candidate;

        /* Append the new consecutive‑sum to the sums array. */
        if (sums_len >= sums_cap) {
            sums_cap = sums_cap * 2;
            sums = (seq_t*)realloc(sums, sums_cap * sizeof(seq_t));
        }
        sums[sums_len++] = terms[terms_len-2] + terms[terms_len-1];
    }

    return terms[n-1];
}
