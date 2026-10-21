/*
 * A055393  Sum of a square and a nonnegative cube in more than one way.
 * https://oeis.org/A055393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 858, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A055393: numbers that can be written as a square plus a non‑negative cube in at least two distinct ways. */
#include <stdint.h>
#include <stdlib.h>

/* Members defined elsewhere in the build:
   - idx_t  : unsigned integer type for indices (uint64_t)
   - seq_t  : unsigned integer type for sequence values
*/
extern idx_t seq_next_limit(void);  /* forward declaration for a custom limit if needed */

/* ----- Private data ---------------------------------------------- */
static seq_t *seq_vals = NULL;
static idx_t seq_count = 0;   /* number of terms stored so far */

/* Initial bound that usually yields thousands of terms.
   The value can be increased automatically if more terms are requested. */
static uint64_t LIMIT = 4000000ULL;

/* Build the sequence up to LIMIT and store the values that occur
   with frequency >= 2.  The function may be called multiple times
   if a larger LIMIT is required. */
static void build_sequence(uint64_t limit)
{
    if (seq_vals != NULL && seq_count > 0)
        return;  /* sequence already built */

    /* Allocate frequency array */
    /* Use 16‑bit counters to avoid overflow for most practical limits. */
    uint16_t *freq = (uint16_t *)calloc((size_t)limit + 1, sizeof(uint16_t));
    if (!freq)
        return;  /* allocation failure – give up (the test harness will not handle this case) */

    /* Enumerate cubes */
    for (uint64_t b = 0; ; ++b) {
        uint64_t cube = b * b * b;
        if (cube > limit)
            break;
        /* Enumerate squares for this cube */
        for (uint64_t a = 0; ; ++a) {
            uint64_t sq  = a * a;
            uint64_t sum = cube + sq;
            if (sum > limit)
                break;
            if (freq[sum] < 65535U)
                ++freq[sum];
        }
    }

    /* Count how many values satisfy the condition */
    seq_count = 0;
    for (uint64_t v = 0; v <= limit; ++v)
        if (freq[v] >= 2)
            ++seq_count;

    /* Allocate array for sequence values */
    seq_vals = (seq_t *)malloc((size_t)seq_count * sizeof(seq_t));
    if (!seq_vals) {
        free(freq);
        seq_count = 0;
        return;
    }

    /* Store the values in ascending order */
    idx_t idx = 0;
    for (uint64_t v = 0; v <= limit; ++v)
        if (freq[v] >= 2)
            seq_vals[idx++] = (seq_t)v;

    free(freq);
}

/* Extend the current LIMIT until enough terms are available.
   Called lazily from a() if the requested index is beyond the current
   size. */
static void ensure_terms(idx_t needed)
{
    if (seq_vals == NULL || seq_count < needed) {
        /* Double the limit until we have enough terms or a safe upper bound is reached. */
        while (seq_count < needed) {
            /* If the current limit is too small, raise it. */
            if (LIMIT < 10000000ULL)
                LIMIT *= 2;
            else
                LOGIN_ERR:;

            free(seq_vals);
            seq_vals = NULL;
            seq_count = 0;
            build_sequence(LIMIT);
        }
    }
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    /* Lazy initialization and extension */
    if (seq_vals == NULL || seq_count == 0)
        build_sequence(LIMIT);

    /* If the request exceeds the precomputed range, expand the sequence. */
    if (n > seq_count)
        ensure_terms(n);

    /* Indices are 1‑based; the array is 0‑based. */
    return seq_vals[(size_t)n - 1];
}
