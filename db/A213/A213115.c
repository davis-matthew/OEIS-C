/*
 * A213115  Number of binary arrays of length n+9 with fewer than 5 ones in
 *          any length 10 subsequence (=less than 50% duty cycle).
 * https://oeis.org/A213115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of binary arrays of length n+9 with fewer than 5 ones in
 *            any consecutive 10 bits. Computation: DP over the 9‑bit suffix;
 *            state = bitmask of the last 9 bits
 * Range:     u8 none, u16 n <= 10, u32 n <= 31, u64 n <= 72, u128 n <= 154,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code is valid in C11 and C++17 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define STATE_COUNT (1U << 9)

/*--------------------------------------------------------------------------*/
/* Transition table:  from each 9‑bit state we can append 0 or 1, keeping the
   total number of 1s in the last 10 bits at most 4. */
static unsigned short trans_to[STATE_COUNT][2];
static unsigned char  trans_outdeg[STATE_COUNT];
static bool trans_inited = false;

/* DP storage: we keep two 512‑element arrays for current and next step. */
static seq_t dp_cur[STATE_COUNT];
static seq_t dp_next[STATE_COUNT];
static bool dp_initialized = false;

/* Counts cache.  The type of the cache differs between C and C++. */
#ifndef __cplusplus
static seq_t *counts_arr  = NULL;
static size_t counts_size = 0;
#else
#include <vector>
static std::vector<seq_t> counts_vec;
#endif

static size_t max_n_comp = 0;  /* largest n already computed */

/*--------------------------------------------------------------------------*/
static void init_transitions(void)
{
    if (trans_inited) return;
    for (unsigned s = 0; s < STATE_COUNT; ++s) {
        unsigned ones = __builtin_popcount(s);
        unsigned out = 0;
        for (unsigned b = 0; b < 2; ++b) {
            unsigned ns = ((s << 1) & (STATE_COUNT - 1)) | b;
            if (ones + b <= 4) {
                trans_to[s][out++] = (unsigned short)ns;
            }
        }
        trans_outdeg[s] = (unsigned char)out;
    }
    trans_inited = true;
}

/*--------------------------------------------------------------------------*/
static void ensure_counts_size(size_t n)
{
#ifndef __cplusplus
    if (counts_size > n) return;
    size_t new_size = counts_size ? counts_size * 2 : 1;
    while (new_size <= n) new_size <<= 1;
    seq_t *new_arr = (seq_t*)realloc(counts_arr, new_size * sizeof(seq_t));
    if (!new_arr) abort();
    for (size_t i = counts_size; i < new_size; ++i) new_arr[i] = (seq_t)0;
    counts_arr  = new_arr;
    counts_size = new_size;
#else
    if (counts_vec.size() > n) return;
    counts_vec.resize(n + 1);   /* default constructed to zero */
#endif
}

/*--------------------------------------------------------------------------*/
static void initialize_first_step(void)
{
    /* Initialise dp_cur to 1 for all 9‑bit states – corresponds to length 9 */
    for (unsigned s = 0; s < STATE_COUNT; ++s) dp_cur[s] = (seq_t)1;
    dp_initialized = true;

    /* Allocate counts cache for at least 1 entry */
#ifndef __cplusplus
    ensure_counts_size(1);
    counts_arr[0] = (seq_t)0; /* unused */
    counts_arr[1] = (seq_t)0; /* placeholder for a(1) */
#else
    ensure_counts_size(1);
    counts_vec[0] = (seq_t)0; /* unused */
    counts_vec[1] = (seq_t)0; /* placeholder for a(1) */
#endif
    max_n_comp = 0;
}

/*--------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined but safe */

    init_transitions();

    if (!dp_initialized) initialize_first_step();

    /* Compute up to requested n if necessary */
    if ((size_t)n > max_n_comp) {
        ensure_counts_size((size_t)n);

        for (size_t step = max_n_comp + 1; step <= (size_t)n; ++step) {
            /* zero dp_next */
            for (unsigned s = 0; s < STATE_COUNT; ++s) dp_next[s] = (seq_t)0;

            /* transition from current states */
            for (unsigned s = 0; s < STATE_COUNT; ++s) {
                seq_t val = dp_cur[s];
                if (val == (seq_t)0) continue;
                unsigned outd = trans_outdeg[s];
                unsigned short *dst = trans_to[s];
                for (unsigned k = 0; k < outd; ++k) {
                    dp_next[dst[k]] += val;
                }
            }

            /* swap dp_cur and dp_next */
            for (unsigned s = 0; s < STATE_COUNT; ++s) dp_cur[s] = dp_next[s];

            /* sum all states: this is a(step) */
            seq_t sum = (seq_t)0;
            for (unsigned s = 0; s < STATE_COUNT; ++s) sum += dp_cur[s];

#ifndef __cplusplus
            counts_arr[step] = sum;
#else
            counts_vec[step] = sum;
#endif
        }
        max_n_comp = n;
    }

#ifndef __cplusplus
    return counts_arr[n];
#else
    return counts_vec[n];
#endif
}
