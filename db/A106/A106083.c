/*
 * A106083  Primes p such that 5*p + 16 and 16*p + 5 are primes.
 * https://oeis.org/A106083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A106083: primes p such that 5*p + 16 and 16*p + 5 are primes.
   The function produces the sequence by searching for primes using the
   provided is_prime(uint64_t) routine.  Results are memoised in a
   static array so that multiple calls in any order are efficient. */
#include <stdlib.h>
#include "../utils/is_prime.h"  /* provides bool is_prime(uint64_t) */

seq_t a(idx_t n)
{
    /* Handle out-of-range indices harmlessly */
    if (n < 1) return 0;

    /* Static state for memoisation */
    static seq_t *terms = NULL;        /* array of discovered terms */
    static idx_t terms_cnt = 0;        /* number of terms stored */
    static idx_t terms_cap = 0;        /* capacity of the array */
    static uint64_t next_candidate = 3;/* next number to test as a prime */

    /* Ensure array has enough capacity to add at least one more term */
    if (terms_cnt == terms_cap) {
        idx_t new_cap = (terms_cap == 0) ? 16 : terms_cap << 1;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* out of memory: abort silently */
        terms = tmp;
        terms_cap = new_cap;
    }

    /* Search until we have at least n terms */
    while (terms_cnt < n) {
        uint64_t p = next_candidate;
        /* Find next prime satisfying the conditions */
        for (;;) {
            if (is_prime(p)) {
                uint64_t a1 = 5 * p + 16;
                uint64_t a2 = 16 * p + 5;
                if (is_prime(a1) && is_prime(a2)) {
                    /* Store the valid prime */
                    terms[terms_cnt++] = (seq_t)p;
                    next_candidate = p + 1;
                    break;
                }
            }
            ++p;
        }
    }

    return terms[n - 1];
}
