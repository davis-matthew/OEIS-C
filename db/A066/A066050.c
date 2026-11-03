/*
 * A066050  Average of divisors of n (sigma(n)/d(n)) is greater than average
 *          of divisors for all k < n.
 * https://oeis.org/A066050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 281, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 76 terms through n = 999 (2026-11-03)
 * Helpers:   mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth positive integer whose average divisor is larger
   than the average divisor of every smaller integer.  Implements
   A066050 by sequential search with memoised records. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/num_divisors.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* static state for memoised computation */
static bool initialized = false;

static seq_t *records = NULL;   /* array of record values */
static size_t rec_cap  = 0;     /* capacity of records */
static size_t rec_cnt  = 0;     /* number of records computed */

static seq_t best_sigma = 0;    /* sigma of current best record */
static uint64_t best_d   = 0;   /* divisor count of current best record */

static idx_t current_n = 0;     /* largest integer already examined */

/* initialise static data structures */
static void init(void)
{
    if (initialized)
        return;

    /* first record is 1 */
    grow_seq(&records, &rec_cap, 1);
    records[0] = (seq_t)1;
    rec_cnt  = 1;

    best_sigma = sigma_of(1); /* equals 1 */
    best_d     = num_divisors(1);
    current_n  = 1;

    initialized = true;
}

/* return the nth term of the sequence (a(1)=1) */
seq_t a(idx_t n)
{
    init();

    while (rec_cnt < n) {
        ++current_n; /* examine next integer */

        seq_t sigma_val = sigma_of(current_n);
        uint64_t d_val  = num_divisors(current_n);

        /* compare averages: sigma_val/d_val  vs  best_sigma/best_d */
        seq_t left  = seq_mul(sigma_val, (seq_t)best_d);
        seq_t right = seq_mul(best_sigma, (seq_t)d_val);

        if (left > right) {            /* new record found */
            if (rec_cnt == rec_cap)
                grow_seq(&records, &rec_cap, rec_cnt + 1);

            records[rec_cnt++] = (seq_t)current_n;

            best_sigma = sigma_val;
            best_d     = d_val;
        }
    }

    return records[n - 1];
}
