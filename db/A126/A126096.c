/*
 * A126096  Primes that are the arithmetic mean of four successive primes.
 * https://oeis.org/A126096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Computes the primes that are the arithmetic mean of four
 *            successive primes by scanning successive primes up to a fixed
 *            upper bound, collecting those whose mean is itself a prime
 * Range:     u8 none, u16 n <= 72, u32 n <= 338, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* static storage for the sequence values */
    static seq_t* seq_arr = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;

    /* primes array and related state */
    static uint64_t* primes_u64 = NULL;
    static size_t primes_cnt = 0;
    static idx_t cur_block_start = 0;
    static bool primes_ready = false;

    /* Prepare primes once with a generous upper bound. */
    if (!primes_ready) {
        /* 20 million is sufficient to cover at least 10 000 terms
           of the sequence while staying far below memory limits. */
        const idx_t LIMIT = 20000000;
        build_primes_upto(LIMIT, &primes_u64, &primes_cnt);
        primes_ready = true;
    }

    /* Generate sequence values until we have the n‑th term. */
    while (seq_len < (size_t)n &&
           cur_block_start <= primes_cnt - (idx_t)4)
    {
        /* Sum the four successive primes. The result fits in seq_t,
           because primes are ≤ 20 million. */
        seq_t sum =
            (seq_t)primes_u64[cur_block_start]     +
            (seq_t)primes_u64[cur_block_start + 1] +
            (seq_t)primes_u64[cur_block_start + 2] +
            (seq_t)primes_u64[cur_block_start + 3];

        /* The mean is an integer only if the sum is divisible by 4. */
        if ((sum & 3U) == 0) {
            seq_t mean = sum >> 2; /* divide by 4 */
            if (is_prime_big(mean)) {
                /* Grow the result array if necessary. */
                if (seq_len == seq_cap) {
                    seq_cap = seq_cap ? (seq_cap << 1) : 16;
                    seq_arr = (seq_t*)realloc(seq_arr,
                                              seq_cap * sizeof(seq_t));
                }
                seq_arr[seq_len++] = mean;
            }
        }

        ++cur_block_start;
    }

    /* The requested term is now available. */
    return seq_arr[n - 1];
}
