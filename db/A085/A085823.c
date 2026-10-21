/*
 * A085823  Numbers in which all substrings are primes.
 * https://oeis.org/A085823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   is_prime_small_uint64, substrings_are_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS sequence A085823 by brute‑forcing primes ≤ 1000
 * and testing that every decimal substring is also prime. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_small_uint64.h"
#include "../utils/substrings_are_prime.h"

/* Static storage for the sequence values (the existing sequence has 9 terms). */
static seq_t seq_values[10];
static size_t seq_length = 0;
static bool seq_ready = false;

/* Build the sequence once. */
static void build_sequence(void)
{
    const uint64_t LIMIT = 1000;   /* sufficient to include all known terms */
    seq_length = 0;

    for (uint64_t cand = 2; cand <= LIMIT; ++cand) {
        if (!is_prime_small_uint64(cand))
            continue;
        if (substrings_are_prime(cand)) {
            seq_values[seq_length++] = (seq_t)cand;
            if (seq_length == 9)   /* all known terms collected */
                break;
        }
    }
    seq_ready = true;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (!seq_ready)
        build_sequence();

    if (n >= 1 && n <= seq_length)
        return seq_values[n - 1];
    /* Undefined beyond known terms – return 0. */
    return (seq_t)0;
}
