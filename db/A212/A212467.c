/*
 * A212467  Number of 0..3 arrays of length n+7 with sum no more than 12 in
 *          any length 8 subsequence (=50% duty cycle).
 * https://oeis.org/A212467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 10, u64 n <= 27, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence is the number of arrays of length n+7 over {0,1,2,3}
 *  such that every contiguous subsequence of length 8 has sum ≤ 12.
 *  (OEIS A212467)
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/*  Number of possible states for the last 7 digits: 4^7 = 16384.  */
#define STATE_SIZE 16384
#define STATE_MASK (STATE_SIZE - 1)

/*  Pre‑computed sum of the 7 digits encoded in each state.  */
static unsigned char sum_vec[STATE_SIZE];
static bool sum_vec_inited = false;

/*  Initialise sum_vec once.  */
static void init_sum_vec(void)
{
    for (size_t s = 0; s < STATE_SIZE; ++s) {
        size_t t = s;
        unsigned sum = 0;
        for (int i = 0; i < 7; ++i) {
            sum += t & 3;
            t >>= 2;
        }
        sum_vec[s] = (unsigned char)sum;
    }
}

/*  Main sequence function.  */
seq_t a(idx_t n)
{
    /*  Total number of positions in the array:  n + 7.  */
    size_t L = (size_t)n + 7;

    /*  Cached results for all lengths up to the current one.  */
    static seq_t *valid          = NULL;
    static size_t valid_size     = 0;
    static size_t computed_up    = 0;   /*  largest L already computed  */

    /*  Dynamic programming tables for transitions from length k‑1 to k.  */
    static seq_t *dp_prev        = NULL;
    static seq_t *dp_next        = NULL;
    static bool   dp_inited      = false;

    /*  Ensure sum_vec is ready.  */
    if (!sum_vec_inited) {
        init_sum_vec();
        sum_vec_inited = true;
    }

    /*  Ensure the cached array is large enough.  */
    if (L >= valid_size) {
        size_t new_size = L + 1;          /* include index 0   */
        seq_t *new_valid = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_valid) return (seq_t)0;  /* allocation failure */
        if (valid) {
            memcpy(new_valid, valid, valid_size * sizeof(seq_t));
            free(valid);
        }
        valid       = new_valid;
        valid_size  = new_size;
    }

    /*  Initialise the zero‑length entry if this is the first use.  */
    valid[0] = (seq_t)1;

    /*  Compute requested lengths up to L.  */
    while (computed_up < L) {
        ++computed_up;

        /*  For lengths 0…7 the count is simply 4^k.  */
        if (computed_up <= 7) {
            if (computed_up == 0) {
                valid[0] = (seq_t)1;
            } else {
                valid[computed_up] = valid[computed_up - 1] * (seq_t)4;
            }
            continue;
        }

        /*  Initialise DP tables once, for length 7.  */
        if (!dp_inited) {
            dp_prev = (seq_t *)malloc(STATE_SIZE * sizeof(seq_t));
            dp_next = (seq_t *)malloc(STATE_SIZE * sizeof(seq_t));
            for (size_t i = 0; i < STATE_SIZE; ++i) dp_prev[i] = (seq_t)1;
            dp_inited = true;
        }

        /*  Clear the next‑state table.  */
        memset(dp_next, 0, STATE_SIZE * sizeof(seq_t));

        /*  Transition from sequences of length (computed_up‑1) to length computed_up.  */
        for (size_t s = 0; s < STATE_SIZE; ++s) {
            seq_t val = dp_prev[s];
            if (!val) continue;
            unsigned sum_s = sum_vec[s];
            for (int x = 0; x < 4; ++x) {
                if (sum_s + (unsigned)x <= 12) {
                    size_t nxt = ((s << 2) | (size_t)x) & STATE_MASK;
                    dp_next[nxt] += val;
                }
            }
        }

        /*  Total count for this length is the sum of all state counts.  */
        seq_t total = (seq_t)0;
        for (size_t i = 0; i < STATE_SIZE; ++i) total += dp_next[i];
        valid[computed_up] = total;

        /*  Prepare for the next iteration.  */
        seq_t *tmp = dp_prev;
        dp_prev = dp_next;
        dp_next = tmp;
    }

    return valid[L];
}
