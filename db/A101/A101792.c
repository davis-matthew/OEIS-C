/*
 * A101792  Primes of the form 8*k-1 such that 4*k-1 and 16*k-1 are also
 *          primes.
 * https://oeis.org/A101792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 43, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 56 terms through n = 9791 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A101792: primes of the form 8*k-1 such that 4*k-1 and 16*k-1 are also primes
   Computation: scan successive integers k starting at 1, test
   4*k-1, 8*k-1 (candidate prime) and 16*k-1 for primality using
   the is_prime utility, and store the candidate whenever all
   conditions hold.  The result set is cached so unordered calls
   to a(idx_t n) are efficient. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

static seq_t *terms     = NULL;   /* cached terms */
static idx_t  terms_cnt = 0;      /* number stored */
static idx_t  capacity  = 0;      /* current array capacity */
static idx_t  current_k = 1;      /* next k to test */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                     /* undefined, not used in OEIS */
    }

    /* Return cached value if already computed */
    if (terms && n <= terms_cnt) {
        return terms[n - 1];
    }

    /* Generate new terms until we reach n */
    while (terms_cnt < n) {
        uint64_t k  = (uint64_t)current_k;
        uint64_t q1 = 4 * k - 1;           /* must be prime */
        if (!is_prime_u64(q1)) {
            current_k++;
            continue;
        }
        uint64_t cand = 8 * k - 1;         /* candidate prime */
        if (!is_prime_u64(cand)) {
            current_k++;
            continue;
        }
        uint64_t q2 = 16 * k - 1;          /* must be prime */
        if (!is_prime_u64(q2)) {
            current_k++;
            continue;
        }

        /* Store the candidate */
        if (terms_cnt >= capacity) {
            idx_t new_cap = (capacity == 0) ? 16 : capacity * 2;
            seq_t *tmp = mem_resize(seq_t, terms, new_cap);
            if (!tmp) {                    /* allocation failure */
                return 0;
            }
            terms = tmp;
            capacity = new_cap;
        }
        terms[terms_cnt++] = (seq_t)cand;
        current_k++;
    }

    return terms[n - 1];
}
