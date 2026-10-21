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
 * Helpers:   divisor_count_u64, grow_seq, seq_mul, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth positive integer whose average divisor is larger
   than the average divisor of every smaller integer.  Implements
   A066050 by sequential search with memoised records. */

#include <stdbool.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/sigma_of.h"
#include "../utils/grow_seq.h"
#include "../utils/seq_mul.h"

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
    best_d     = divisor_count_u64(1);
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
        uint64_t d_val  = divisor_count_u64(current_n);

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
