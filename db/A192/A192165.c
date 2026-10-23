/*
 * A192165  Monotonic ordering of nonnegative differences 4^i-7^j, for 40>=
 *          i>=0, j>=0.
 * https://oeis.org/A192165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 23, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A192165 – monotonic ordering of all non‑negative differences
 * 4^i - 7^j with 0 ≤ i ≤ 40 and j ≥ 0, sorted and deduplicated. */

/* The build process generates all pairs (i,j) with 4^i ≥ 7^j,
 * stores the differences, sorts them, removes duplicates,
 * and keeps the result in a static array.  Subsequent calls to
 * a(idx_t n) simply return the nth element (1‑based).  The code
 * works for both the plain integer builds (seq_t is an unsigned type)
 * and for the arbitrary‑precision build (seq_t is a bigint class). */

#include <stdlib.h>
#include <stdbool.h>

/* Tables for the differences and the final sorted unique values. */
#define MAX_POW4       41          /* i from 0 to 40 inclusive */
#define MAX_RAW       5000          /* generous capacity for all raw diffs */
#define MAX_UNIQ      3000          /* capacity for unique sorted values */

static seq_t raw_vals[MAX_RAW];
static size_t raw_len = 0;

static seq_t uniq_vals[MAX_UNIQ];
static size_t uniq_len = 0;

static bool built = false;

/* Comparison function for qsort.  Works for both integer and bigint types. */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *pa = (const seq_t *)a;
    const seq_t *pb = (const seq_t *)b;
    if (*pa < *pb) return -1;
    if (*pa > *pb) return 1;
    return 0;
}

/* Build the entire sequence once. */
static void build_sequence(void)
{
    if (built) return;

    /* Pre‑compute powers of 4: 4^i for i = 0 .. 40. */
    seq_t pow4[MAX_POW4];
    pow4[0] = (seq_t)1;
    for (idx_t i = 1; i < MAX_POW4; ++i)
        pow4[i] = pow4[i - 1] * ((seq_t)4);

    /* Generate all differences 4^i - 7^j >= 0. */
    raw_len = 0;
    for (idx_t i = 0; i < MAX_POW4; ++i) {
        seq_t v4 = pow4[i];
        seq_t v7 = (seq_t)1;
        while (v7 <= v4) {
            if (raw_len == MAX_RAW) {
                /* Should not happen for i ≤ 40; capacity is generous. */
                return;
            }
            raw_vals[raw_len++] = v4 - v7;
            v7 *= ((seq_t)7);
        }
    }

    /* Sort the raw differences. */
    qsort(raw_vals, raw_len, sizeof(seq_t), cmp_seq);

    /* Extract unique values into uniq_vals. */
    uniq_len = 0;
    for (size_t k = 0; k < raw_len; ++k) {
        if (uniq_len == 0 || raw_vals[k] != uniq_vals[uniq_len - 1]) {
            if (uniq_len == MAX_UNIQ) break; /* safety */
            uniq_vals[uniq_len++] = raw_vals[k];
        }
    }

    built = true;
}

/* Public function. */
seq_t a(idx_t n)
{
    /* Build the sequence on the first call. */
    if (!built) build_sequence();

    if (n < 1) return (seq_t)0;  /* undefined for n = 0 */

    /* Convert to zero‑based index safely. */
    if (n - 1 < uniq_len)
        return uniq_vals[n - 1];

    /* Out‑of‑range requests return 0 (sequence is finite). */
    return (seq_t)0;
}
