/*
 * A079017  Suppose p and q = p+14 are primes. Define the difference pattern
 *          of (p,q) to be the successive differences of the primes in the
 *          range p to q. There are 15 possible difference patterns, namely
 *          [14], [2,12], [6,8], [8,6], [12,2], [2,4,8], [2,6,6], [2,10,2],
 *          [6,2,6], [6,6,2], [8,4,2], [2,4,6,2], [2,...
 * https://oeis.org/A079017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A079017 – the minimal prime p such that p and p+14 are prime,
   for each of the 15 possible difference patterns.
   This implementation sieves primes up to a fixed limit,
   finds the minimal p for each pattern, sorts the 15 values,
   and returns the n‑th term (0 for n<1 or n>15). */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "../utils/cmp_seq_t.h"

/* number of distinct difference patterns for a 14‑prime tuple */
enum { NUM_PATTERNS = 15 };

/* pattern description: length of the difference list and the
   successive differences (up to 5 elements). */
static const struct {
    int len;
    int diff[5];
} patterns[NUM_PATTERNS] = {
    {1, {14, 0, 0, 0, 0}},
    {2, {2, 12, 0, 0, 0}},
    {2, {6, 8, 0, 0, 0}},
    {2, {8, 6, 0, 0, 0}},
    {2, {12, 2, 0, 0, 0}},
    {3, {2, 4, 8, 0, 0}},
    {3, {2, 6, 6, 0, 0}},
    {3, {2,10, 2, 0, 0}},
    {3, {6, 2, 6, 0, 0}},
    {3, {6, 6, 2, 0, 0}},
    {3, {8, 4, 2, 0, 0}},
    {4, {2, 4, 6, 2, 0}},
    {4, {2, 6, 4, 2, 0}},
    {5, {2, 2, 4, 2, 4}},
    {5, {2, 4, 2, 4, 2}}
};

/* external entry point required by the test harness */
seq_t a(idx_t n)
{
    static bool ready = false;
    static seq_t minp[NUM_PATTERNS];
    static seq_t sorted[NUM_PATTERNS];
    static bool found[NUM_PATTERNS];
    static int found_count = 0;

    if (!ready) {
        /* initialise arrays */
        for (int i = 0; i < NUM_PATTERNS; ++i) {
            minp[i] = (seq_t)(-1);
            found[i] = false;
        }

        /* sieve limit – enough to capture all 15 minimal primes
           (the largest known minimal p is 1997). */
        const idx_t LIMIT = 200000;
        uint8_t *prime_flag = (uint8_t *)malloc((size_t)LIMIT + 1);
        if (!prime_flag) exit(EXIT_FAILURE);
        for (size_t i = 0; i <= (size_t)LIMIT; ++i) prime_flag[i] = 1;
        prime_flag[0] = prime_flag[1] = 0;
        for (size_t i = 2; i * i <= (size_t)LIMIT; ++i) {
            if (prime_flag[i]) {
                for (size_t j = i * i; j <= (size_t)LIMIT; j += i)
                    prime_flag[j] = 0;
            }
        }

        /* search for minimal p for each pattern */
        for (size_t p = 2; p + 14 <= (size_t)LIMIT; ++p) {
            if (!prime_flag[p] || !prime_flag[p + 14])
                continue;

            /* collect primes in [p, p+14] */
            int primes[15];
            int pc = 0;
            for (size_t x = p; x <= p + 14; ++x) {
                if (prime_flag[x])
                    primes[pc++] = (int)x;
            }
            if (pc < 2) continue;          /* safe guard */

            int dlen = pc - 1;
            int diffs[5];
            for (int i = 0; i < dlen; ++i)
                diffs[i] = primes[i + 1] - primes[i];

            /* match against patterns */
            for (int pi = 0; pi < NUM_PATTERNS; ++pi) {
                if (patterns[pi].len != dlen)
                    continue;
                bool match = true;
                for (int k = 0; k < dlen; ++k) {
                    if (patterns[pi].diff[k] != diffs[k]) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    if (!found[pi] || (seq_t)p < minp[pi]) {
                        if (!found[pi])
                            ++found_count;
                        minp[pi] = (seq_t)p;
                        found[pi] = true;
                    }
                    break; /* one pattern per prime pair */
                }
            }

            if (found_count == NUM_PATTERNS)
                break; /* all patterns found */
        }
        free(prime_flag);

        /* sort minimal p's ascending */
        memcpy(sorted, minp, sizeof(minp));
        qsort(sorted, NUM_PATTERNS, sizeof(seq_t), cmp_seq_t);
        ready = true;
    }

    if (n < 1 || n > NUM_PATTERNS)
        return 0;
    return sorted[(size_t)n - 1];
}
