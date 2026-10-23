/*
 * A100726  Prime numbers whose binary representations are split into a
 *          maximum of 7 runs.
 * https://oeis.org/A100726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A100726 – primes whose binary representation consists of at most 7 runs.
/// Computation: iterate over successive integers, test for primality, count runs
/// in binary representation, and collect those satisfying the condition.  Uses
/// incremental generation, memoized between calls, and re‑allocates the
/// storage when necessary. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/is_prime.h"

/* Count the number of runs in the binary representation of x.
   A run is a maximal block of consecutive equal bits.  */
static int count_runs(uint64_t x)
{
    /* Find most significant set bit position. */
    int pos = 63 - __builtin_clzll(x);   /* x > 0 for primes */
    int runs = 1;
    int prev = (x >> pos) & 1u;
    /* Scan remaining bits. */
    for (int i = pos - 1; i >= 0; --i) {
        int bit = (x >> i) & 1u;
        if (bit != prev) {
            ++runs;
            prev = bit;
        }
    }
    return runs;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static storage for previously generated primes. */
    static seq_t *list = NULL;
    static size_t cap   = 0;
    static size_t cnt   = 0;
    static uint64_t next_candidate = 0;

    /* Initialize on first call. */
    if (list == NULL) {
        cap = 128;
        list = (seq_t *)malloc(cap * sizeof(seq_t));
        next_candidate = 2;          /* start from the first prime */
    }

    /* Grow and generate until we have at least n terms. */
    while (cnt < (size_t)n) {
        uint64_t x = next_candidate;

        if (x >= 2 && is_prime(x)) {
            if (count_runs(x) <= 7) {
                /* Ensure capacity. */
                if (cnt >= cap) {
                    cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(list, cap * sizeof(seq_t));
                    if (tmp == NULL) abort();
                    list = tmp;
                }
                list[cnt++] = (seq_t)x;
            }
        }

        /* Advance to next candidate (skip even numbers after 2). */
        if (x <= 2) {
            next_candidate = 3;
        } else {
            next_candidate += 2;
        }
    }

    return list[(size_t)n - 1];
}
