/*
 * A106070  Primes p such that 3*p + 10 and 10*p + 3 are primes.
 * https://oeis.org/A106070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime p such that 3*p + 10 and 10*p + 3 are primes
 * Range:     u8 n <= 21, u16 n <= 797, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* C/C++ compatibility for realloc/malloc/calloc casts */
#if defined(__cplusplus)
#  define SEQ_RECAST(p) reinterpret_cast<seq_t*>(p)
#  define ARR_RECAST(p) reinterpret_cast<uint8_t*>(p)
#  define MUL_RECAST(p) reinterpret_cast<unsigned int*>(p)
#else
#  define SEQ_RECAST(p) (seq_t*)(p)
#  define ARR_RECAST(p) (uint8_t*)(p)
#  define MUL_RECAST(p) (unsigned int*)(p)
#endif

/* Storage for up to n terms */
static seq_t *seq      = NULL;
static idx_t   seq_sz  = 0;
static idx_t   seq_cap = 0;

/* Push a new term into the sequence, expanding the array on demand */
static void seq_push(seq_t val)
{
    if (seq_sz == seq_cap) {
        idx_t new_cap = seq_cap ? seq_cap * 2ULL : 1ULL;
        seq = SEQ_RECAST(realloc(seq, new_cap * sizeof(seq_t)));
        seq_cap = new_cap;
    }
    seq[seq_sz++] = val;
}

/* Find the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Incrementally enumerate primes and test the condition. */
    static uint64_t cand = 2;   /* current integer to test for primality */

    /* Generate terms until we have the n‑th. */
    while (seq_sz < n) {
        /* Skip non‑primes. */
        while (!is_prime(cand))
            ++cand;

        /* cand is prime. Check the two derived primes. */
        if (is_prime(3ULL * cand + 10ULL) &&
            is_prime(10ULL * cand + 3ULL)) {
            seq_push((seq_t)cand);
        }
        ++cand;
    }

    return seq[n - 1ULL];
}
