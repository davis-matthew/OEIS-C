/*
 * A103850  Primes with digits not in all previous terms.
 * https://oeis.org/A103850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   digit_mask, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A103850 – minimal prime whose decimal digits have not appeared in any
   previous term; the sequence stops when all 10 digits 0–9 have been used.
   Computation is performed by a simple search: iterate through primes,
   keep a bitmask of digits already used, and accept a prime only if its
   digit set is disjoint from that mask. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* bool is_prime(uint64_t); */
#include "../utils/digit_mask.h" /* uint16_t digit_mask(seq_t x); */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* undefined for n < 1 */

    /* static state for memoisation */
    static seq_t terms[32];                 /* sequence storage (max. 10 terms) */
    static idx_t count = 0;                 /* how many terms computed so far */
    static uint16_t used_mask = 0;          /* digits used so far (bits 0‑9) */
    static uint64_t next_candidate = 2;     /* next number to test for primality */

    /* generate terms until we have at least n of them or the sequence ends */
    while (count < n && used_mask != 0x3FF) {
        uint64_t cand = next_candidate;
        bool found = false;

        /* search for the next prime */
        while (!found) {
            if (cand == 2) {
                if (is_prime(2)) found = true;
            } else {
                if ((cand & 1ULL) && is_prime(cand)) found = true;
            }
            if (!found) {
                if (cand == 2) cand = 3;
                else cand += 2;           /* skip even numbers > 2 */
            }
        }

        /* accept the prime only if its digits are new */
        uint16_t mask = digit_mask((seq_t)cand);
        if ((mask & used_mask) == 0) {
            terms[count] = (seq_t)cand;
            used_mask |= mask;
            count++;
        }

        /* next candidate for the following search */
        next_candidate = (cand == 2 ? 3 : cand + 2);
    }

    /* return the requested term if it exists */
    if (n <= count) return terms[n - 1];

    /* sequence ended before reaching index n */
    return (seq_t)0;
}
