/*
 * A075592  Numbers n such that number of distinct prime divisors of n is a
 *          divisor of n.
 * https://oeis.org/A075592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   numbers n such that ω(n) (count of distinct prime factors)
 *            divides n
 * Range:     u8 n <= 182, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_distinct_primes.h"

/* cache of sequence terms */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* last integer that has been tested */
static uint64_t tested_up_to = 1;

/* add a qualifying number to the cache */
static void add_term(seq_t val)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? (seq_cap << 1) : 8;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
    }
    seq_cache[seq_len++] = val;
}

/* main function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 0 */

    while (seq_len < (size_t)n) {
        ++tested_up_to;                   /* next integer to test */

        /* Skip the first positive integer (1) since omega(1)=0 */
        if (tested_up_to < 2) continue;

        int omega = count_distinct_primes(tested_up_to);
        if (omega != 0 && (tested_up_to % (uint64_t)omega) == 0)
            add_term((seq_t)tested_up_to);
    }
    return seq_cache[(size_t)n - 1];
}
