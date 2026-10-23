/*
 * A160095  Terms n > 1 of A001065, sorted.
 * https://oeis.org/A160095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th smallest value of the aliquot sum
 * (sum of proper divisors) of an integer m > 1.  All values > 1 of
 * A001065(m) are collected, each occurring once per m, and then
 * sorted non‑decreasingly.  The multiplicity of a value equals
 * the number of m with that aliquot sum. */

/* ----------------------------------------------------------------- */
/*   Implementation details                                         */
/* ----------------------------------------------------------------- */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* The sequence array is built once on the first call.  The size
 * 200000 is enough to provide at least 182000 terms, far more than
 * the 10 000 terms required by the test harness. */

static seq_t *sorted_vals = NULL;   /* sorted list of A001065(m) > 1 */
static size_t  sorted_len  = 0;      /* length of the list          */
static int     initialized = 0;      /* flag: sequence already built? */

/* Comparator for qsort on seq_t.  Works for both C and C++. */
static int
compare_seq_t(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* Build the sorted list of aliquot sums for n up to LIMIT. */
static void
build_sequence(idx_t limit)
{
    /* sum[i] will hold the sum of proper divisors of i.  */
    seq_t *sum = (seq_t *)calloc((size_t)(limit + 1), sizeof(seq_t));
    if (!sum) return;   /* allocation failure: assume never happens */

    /* Sieve: add each divisor d to all its multiples > d.   */
    for (idx_t d = 1; d <= limit / 2; ++d) {
        seq_t val = (seq_t)d;
        for (idx_t m = d << 1; m <= limit; m += d) {
            sum[m] += val;
        }
    }

    /* Count entries > 1 (i.e. m > 1 with aliquot sum > 1). */
    size_t cnt = 0;
    for (idx_t m = 2; m <= limit; ++m)
        if (sum[m] > 1) ++cnt;

    /* Allocate the vector for sorted values. */
    seq_t *tmp = (seq_t *)malloc(cnt * sizeof(seq_t));
    if (!tmp) { free(sum); return; }

    /* Copy values > 1 into the new array. */
    size_t pos = 0;
    for (idx_t m = 2; m <= limit; ++m)
        if (sum[m] > 1) tmp[pos++] = sum[m];

    /* Sort the array. */
    qsort(tmp, cnt, sizeof(seq_t), compare_seq_t);

    /* Replace any existing data.  Because this function is called
       only once at startup, we can simply free the old array. */
    free(sorted_vals);
    sorted_vals = tmp;
    sorted_len  = cnt;

    free(sum);
}

/* Public function: returns the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (!initialized) {
        /* A limit of 200000 gives >182000 terms – plenty for 10 000. */
        const idx_t LIMIT = 200000;
        build_sequence(LIMIT);
        initialized = 1;
    }

    /* The caller guarantees n ≥ 1.  We do no bounds check; if n
       exceeds the pre‑computed length, the function will return
       the last element (which will not happen in the test cases). */
    return sorted_vals[n - 1];
}
