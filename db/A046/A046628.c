/*
 * A046628  Distinct odd numbers in writing first numerator and then
 *          denominator of each element to the right of the central elements
 *          of the 1/5-Pascal triangle (by row).
 * https://oeis.org/A046628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 46, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A046628: Distinct odd numbers in writing first numerator and then denominator
   of each element to the right of the central elements of the 1/5‑Pascal triangle.
   Formula: a(n) = the nth distinct odd integer occurring in the right
   half of the 1/5‑Pascal triangle (C( n, k ) / 5^v5(C(n,k)) reduced to lowest
   terms).  */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Helper struct to hold a fraction with numerator (seq_t) and
   denominator 1 or 5. */
typedef struct {
    seq_t num;
    unsigned den;          /* only 1 or 5 */
} Fraction;

/* ------------------------------------------------------------------ */
/* Fixed maximum cache size.  20000 is ample for the required limits. */
#define MAX_CACHE 20000

/* ------------------------------------------------------------------ */
/* Storage for the sequence values and the set of values that have already been seen. */
static seq_t *seq_vals  = NULL;  /* distinct odd numbers, indexed by a(n)-1 */
static size_t seq_len   = 0;     /* number of values stored */

static seq_t *seen      = NULL;  /* all values that have ever been encountered */
static size_t seen_cnt  = 0;     /* number of seen values */

/* ------------------------------------------------------------------ */
/* Current row of the 1/5‑Pascal triangle. */
static Fraction *row      = NULL;  /* array of Fractions */
static size_t cur_len      = 0;     /* length of current row */

/* ------------------------------------------------------------------ */
/* Check whether a value has already been seen.  Linear search because
   the table size is small (≤ MAX_CACHE). */
static bool seen_contains(seq_t val)
{
    for (size_t i = 0; i < seen_cnt; ++i)
        if (seen[i] == val)
            return true;
    return false;
}

/* ------------------------------------------------------------------ */
/* Append a new odd value if it hasn't been seen before. */
static void add_value(seq_t val)
{
    if (seen_contains(val))
        return;

    /* Store in the sequence array. */
    seq_vals[seq_len++] = val;

    /* Store in the seen array. */
    seen[seen_cnt++] = val;
}

/* ------------------------------------------------------------------ */
/* Generate the next row of the 1/5‑Pascal triangle, process its right
   side, and add new distinct odd numbers to the sequence. */
static void generate_next_row(void)
{
    size_t new_len;
    Fraction *new_row;

    /* Handle the first two transitions specially. */
    if (cur_len == 1) {           /* 1 → 1 1 */
        new_len = 2;
        new_row = (Fraction *)malloc(new_len * sizeof(Fraction));
        new_row[0].num = (seq_t)1;
        new_row[0].den = 1;
        new_row[1].num = (seq_t)1;
        new_row[1].den = 1;
    } else if (cur_len == 2) {    /* 1 1 → 1 1/5 1 */
        new_len = 3;
        new_row = (Fraction *)malloc(new_len * sizeof(Fraction));
        new_row[0].num = (seq_t)1;
        new_row[0].den = 1;
        new_row[1].num = (seq_t)1;
        new_row[1].den = 5;
        new_row[2].num = (seq_t)1;
        new_row[2].den = 1;
    } else {                      /* general recurrence */
        new_len = cur_len + 1;
        new_row = (Fraction *)malloc(new_len * sizeof(Fraction));

        /* boundaries are 1/1 */
        new_row[0].num = (seq_t)1;
        new_row[0].den = 1;
        new_row[new_len - 1].num = (seq_t)1;
        new_row[new_len - 1].den = 1;

        /* interior entries: sum of two above, reduce modulo 5 */
        for (size_t j = 1; j < new_len - 1; ++j) {
            const Fraction *pl = &row[j - 1];
            const Fraction *pr = &row[j];

            seq_t left  = pl->num;
            if (pl->den == 1) left *= 5;      /* bring to denominator 5 */

            seq_t right = pr->num;
            if (pr->den == 1) right *= 5;

            seq_t sum = left + right;
            if (sum % 5 == 0) {
                sum /= 5;
                new_row[j].den = 1;
            } else {
                new_row[j].den = 5;
            }
            new_row[j].num = sum;
        }
    }

    /* Process the right side: indices from new_len/2 to new_len-1. */
    size_t start_idx = new_len / 2;
    for (size_t i = start_idx; i < new_len; ++i) {
        const Fraction *f = &new_row[i];

        /* numerator */
        if ((f->num % 2) == 1)
            add_value(f->num);

        /* denominator (always 1 or 5) */
        seq_t den = f->den;
        if ((den % 2) == 1)
            add_value(den);
    }

    /* Advance to the new row. */
    free(row);
    row = new_row;
    cur_len = new_len;
}

/* ------------------------------------------------------------------ */
/* Public sequence function. */
seq_t a(idx_t n)
{
    /* First initialisation on the first call. */
    static bool initialized = false;
    if (!initialized) {
        initialized = true;

        /* Allocate fixed-size buffers for a reliable implementation. */
        seq_vals = (seq_t *)malloc(MAX_CACHE * sizeof(seq_t));
        seen     = (seq_t *)malloc(MAX_CACHE * sizeof(seq_t));

        /* Start sequence with a(1) = 1. */
        seq_vals[0] = (seq_t)1;
        seq_len = 1;

        /* Mark 1 as seen. */
        seen[0] = (seq_t)1;
        seen_cnt = 1;

        /* Initialise first row of the triangle: 1. */
        cur_len = 1;
        row = (Fraction *)malloc(sizeof(Fraction));
        row[0].num = (seq_t)1;
        row[0].den = 1;
    }

    /* Generate rows until we have produced at least n distinct odd numbers. */
    while (seq_len < (size_t)n) {
        generate_next_row();
    }

    return seq_vals[n - 1];
}
