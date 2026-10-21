/*
 * A059981  Order of compositeness for the n-th composite number.
 * https://oeis.org/A059981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the length of the composite‑index chain for the n‑th
 *            composite number. a(n) = 1 + m where m is the largest number
 *            such that S(S(...S(c(n))...)) with m applications of S gives a
 *            composite
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n ≥ 1.  The sequence is pre‑computed up to a fixed limit
   sufficient for all tests (here 3 000 000).  Subsequent calls are O(1). */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>   /* for memset */

#define MAX_LIMIT 3000000   /* must be ≥ largest composite needed for tests */

static bool is_prime[MAX_LIMIT + 1];
static uint32_t comp_index[MAX_LIMIT + 1];
static uint32_t composites[MAX_LIMIT];  /* at most MAX_LIMIT composites listed */
static idx_t comp_cnt = 0;
static bool initialized = false;

seq_t a(idx_t n) {
    /* One‑time sieve and precomputation */
    if (!initialized) {
        /* prime table */
        for (uint32_t i = 0; i <= MAX_LIMIT; ++i) is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;

        /* sieve */
        for (uint32_t p = 2; (uint64_t)p * p <= MAX_LIMIT; ++p) {
            if (is_prime[p]) {
                for (uint64_t m = (uint64_t)p * p; m <= MAX_LIMIT; m += p)
                    is_prime[(uint32_t)m] = false;
            }
        }

        /* initialise comp_index with zeros */
        memset(comp_index, 0, sizeof(comp_index));

        /* collect composites and their indices */
        for (uint32_t i = 4; i <= MAX_LIMIT; ++i) {
            if (!is_prime[i]) {
                comp_index[i] = (uint32_t)(comp_cnt + 1); /* 1‑based */
                composites[comp_cnt++] = i;
            }
        }
        initialized = true;
    }

    /* Guard: if n exceeds number of composites pre‑computed, return 0 */
    if (n > comp_cnt) return 0;

    seq_t steps = 0;
    uint32_t current = composites[n - 1];   /* n‑th composite number */

    /* walk the S‑chain while the intermediate values are composite */
    while (current <= MAX_LIMIT && comp_index[current] != 0) {
        steps++;                         /* count this step */
        current = comp_index[current];   /* next value is the composite index */
    }

    return steps;
}
