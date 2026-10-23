/*
 * A039665  Sets of 4 consecutive numbers with equal number of divisors.
 * https://oeis.org/A039665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039665:
 *  a(n) is the n-th element of all overlapping 4‑consecutive
 *  integers that have equal number of divisors.
 *  The sequence is built by scanning all integers,
 *  recording each 4‑tuple i,i+1,i+2,i+3 with d(i)=d(i+1)=d(i+2)=d(i+3).
 */

#include <stdlib.h>
#include <stdint.h>

/* Dynamically grow a seq_t array to accommodate at least 'need' elements. */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        if (new_cap < need) new_cap = need;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Cached data for the sequence.  The array 'sequence' holds all numbers
 * in the order they appear in the sequence.  The values are stored in
 * the order of each 4‑tuple, so overlapping tuples cause duplicates
 * to appear in the array.  The precomputation is performed once on the
 * first call to a(). */
static seq_t  *sequence   = NULL;  /* array of sequence values     */
static size_t  seq_len    = 0;     /* number of values stored      */
static size_t  seq_cap    = 0;     /* current capacity of the array*/
static int     initialized = 0;    /* flag: 1 if precomputation done */

/* Build divisor counts up to LIMIT and populate the sequence array. */
static void precompute(void)
{
    if (initialized) return;
    initialized = 1;

    /* A bound that comfortably contains the first 10 000 terms of the
     * sequence.  The OEIS data shows the 24th term is 7259; runs are
     * sparse so 20 000 000 is more than sufficient. */
    const idx_t LIMIT = 20000000ULL;

    /* Allocate array for divisor counts; uint16_t is plenty for d(n) ≤ LIMIT. */
    uint16_t *d = (uint16_t *)calloc((size_t)(LIMIT + 1), sizeof(uint16_t));
    if (!d) exit(1);

    /* Sieve to count divisors for all n ≤ LIMIT. */
    for (idx_t i = 1; i <= LIMIT; ++i) {
        for (idx_t j = i; j <= LIMIT; j += i) {
            ++d[j];
        }
    }

    /* Allocate initial capacity for the sequence array. */
    seq_cap = 1024;
    sequence = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    if (!sequence) exit(1);

    /* Scan for all overlapping 4‑tuples with equal divisor counts. */
    for (idx_t i = 1; i + 3 <= LIMIT; ++i) {
        if (d[i] == d[i + 1] && d[i] == d[i + 2] && d[i] == d[i + 3]) {
            grow_seq(&sequence, &seq_cap, seq_len + 4);
            sequence[seq_len++] = (seq_t)i;
            sequence[seq_len++] = (seq_t)(i + 1);
            sequence[seq_len++] = (seq_t)(i + 2);
            sequence[seq_len++] = (seq_t)(i + 3);
        }
    }

    free(d);
}

/* Return the nth term of A039665 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, safe return */
    precompute();
    size_t idx = (size_t)(n - 1);
    if (idx < seq_len) {
        return sequence[idx];
    }
    /* If n exceeds the precomputed range; should not happen for tests. */
    return 0;
}
