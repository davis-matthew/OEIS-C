/*
 * A106071  Primes p such that 3*p + 14 and 14*p + 3 are primes.
 * https://oeis.org/A106071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: primes p such that 3p+14 and 14p+3 are primes */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* dynamic array of found terms */
static seq_t *seq_vals = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

/* current candidate prime */
static uint64_t candidate = 2ULL;

/* append a value to |seq_vals|, growing the array if necessary */
static void push_seq(seq_t v)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 16;
        seq_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
    seq_vals[seq_len++] = v;
}

/* fill |seq_vals| until it contains at least |n| elements */
static void ensure_terms(idx_t n)
{
    if (seq_len >= (size_t)n) return;

    while (seq_len < (size_t)n) {
        /* skip until next prime candidate */
        while (1) {
            if (candidate <= 2ULL) {
                if (candidate == 2ULL) break;
                candidate = 3ULL;          /* start with first odd >2 */
                continue;
            }
            if ((candidate & 1ULL) == 0ULL) {   /* even numbers are not prime */
                ++candidate;
                continue;
            }
            if (is_prime(candidate)) break;     /* candidate is prime */
            candidate += 2ULL;                 /* next odd number */
        }

        uint64_t p = candidate;
        candidate += 2ULL;  /* advance for next search */

        /* test the two linear forms */
        uint64_t a = 3ULL * p + 14ULL;
        uint64_t b = 14ULL * p + 3ULL;
        if (is_prime(a) && is_prime(b))
            push_seq((seq_t)p);
    }
}

/* external linkage as required by the judge */
seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* behaviour below offset is irrelevant */
    ensure_terms(n);
    return seq_vals[n - 1];
}
