/*
 * A137873  Prime numbers, isolated from neighboring primes by more than 12.
 * https://oeis.org/A137873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 343, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS sequence A137873
   Prime numbers that are isolated from neighboring primes by more than 12.
   The implementation relies on the supplied next_prime() utility and
   searches for primes sequentially, storing isolated primes in a
   dynamically growing array.                                      */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef __cplusplus
/* C-only helpers ----------------------------------------------------- */
static seq_t *isolated = NULL;
static size_t isolated_cap = 0;
static size_t isolated_cnt = 0;

/* Append value to isolated array ------------------------------------- */
static void push_seq(uint64_t v)
{
    if (isolated_cnt >= isolated_cap) {
        isolated_cap = isolated_cap ? isolated_cap * 2 : 16;
        isolated = (seq_t *)realloc(isolated,
                                    isolated_cap * sizeof(seq_t));
    }
    isolated[isolated_cnt++] = (seq_t)v;
}
#define ISOLATED_CNT isolated_cnt
#define PUSH(v)      push_seq((v))
#else
/* C++-only helpers ---------------------------------------------------- */
#include <vector>
static std::vector<seq_t> isolatedVec;
#define ISOLATED_CNT isolatedVec.size()
#define PUSH(v)      isolatedVec.push_back((seq_t)(v))
#endif

/* Utility to get the next prime (from the provided utils) ------------- */
#include "../utils/next_prime.h"

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* OEIS offset starts at 1 */

    static uint64_t prev_prime = 0;
    static uint64_t cur_prime  = 0;
    static bool initialized = false;

    if (!initialized) {
        cur_prime = next_prime(2); /* first prime = 2 */
        prev_prime = 0;
        initialized = true;
    }

    /* Generate isolated primes until we have at least n items */
    while (ISOLATED_CNT < (size_t)n) {
        uint64_t next_p = next_prime(cur_prime + 1);

        if (prev_prime != 0 &&
            (cur_prime - prev_prime > 12) &&
            (next_p - cur_prime > 12)) {
            PUSH(cur_prime);
        }

        prev_prime = cur_prime;
        cur_prime  = next_p;
    }

    /* Retrieve the (n-1)-th element (offset 1) */
#ifdef __cplusplus
    return isolatedVec[n - 1];
#else
    return isolated[n - 1];
#endif
}
