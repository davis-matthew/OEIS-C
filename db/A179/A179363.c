/*
 * A179363  Number of permutations of 1..n with i-10<=p(i)<=i+3.
 * https://oeis.org/A179363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count permutations of 1..n with i-10 <= p(i) <= i+3 for all i. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define MASK 14
#define STATE_SIZE ((size_t)1 << MASK)
#define FULL_MASK ((uint32_t)STATE_SIZE - 1)

static const seq_t ONE = (seq_t)1;

seq_t a(idx_t n)
{
    /* Allocate DP tables with zeroed memory. */
    seq_t *dp   = (seq_t *)calloc(STATE_SIZE, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(STATE_SIZE, sizeof(seq_t));
    if (!dp || !next) {            // OOM guard
        free(dp);
        free(next);
        return (seq_t)0;
    }

    /* Build the initial mask for i = 1.  Bits represent values
       i-10 + k, k = 0..13.  If a value is outside [1..n], the bit
       is set to 1 (already used/out-of-range). */
    uint32_t init_mask = 0;
    for (uint32_t k = 0; k < MASK; ++k) {
        idx_t v = (idx_t)(1 - 10 + k);
        if (v < (idx_t)1 || v > n) {
            init_mask |= (1u << k);
        }
    }
    dp[init_mask] = ONE;

    /* DP over positions i = 1 .. n. */
    for (idx_t i = 1; i <= n; ++i) {
        /* If i+4 > n, the value i+4 will be out of range in the next
           window; we force its bit (position 13) to 1 after shifting. */
        uint32_t out_next = ((i + (idx_t)4) > n) ? (1u << 13) : 0u;

        /* Clear next table */
        for (size_t m = 0; m < STATE_SIZE; ++m) {
            next[m] = (seq_t)0;
        }

        for (size_t m = 0; m < STATE_SIZE; ++m) {
            seq_t ways = dp[m];
            if (!ways) continue;

            uint32_t free_bits = (~(uint32_t)m) & FULL_MASK;
            while (free_bits) {
                uint32_t lsb  = free_bits & -free_bits;
                uint32_t bit  = (uint32_t)__builtin_ctz((unsigned)free_bits);
                free_bits ^= lsb;

                uint32_t next_mask = ((m | (1u << bit)) >> 1) | out_next;
                next[next_mask] += ways;
            }
        }

        /* Swap tables for next iteration */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    /* Sum counts over all final states. */
    seq_t result = (seq_t)0;
    for (size_t m = 0; m < STATE_SIZE; ++m) {
        result += dp[m];
    }

    free(dp);
    free(next);
    return result;
}
