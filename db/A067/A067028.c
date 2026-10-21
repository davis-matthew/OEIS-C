/*
 * A067028  Numbers with a composite number of prime factors (counted with
 *          multiplicity).
 * https://oeis.org/A067028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_prime_factors, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A067028: numbers with a composite number of prime factors
 * (counted with multiplicity).  Computed by sequential search. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/count_prime_factors.h"
#include "../utils/isPrimeGeneric.h"

/* Return true if x has a composite big‑omega value. */
static inline bool composite_omega(uint64_t x)
{
    if (x <= 1) return false;          /* Ω(1) = 0, not composite */
    seq_t omega = count_prime_factors((seq_t)x);
    if (omega <= (seq_t)1) return false;/* Ω = 1 is not composite */
    return !isPrimeGeneric(omega);    /* composite iff Ω is not prime */
}

/* Return the n-th term of A067028. */
seq_t a(idx_t n)
{
    /* Persistent state */
    static seq_t *seq   = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t cand   = 1;   /* next integer to test */

    /* Extend the sequence until it contains at least n terms. */
    while (len < n) {
        if (cand > 1 && composite_omega((uint64_t)cand)) {
            /* Grow storage if needed. */
            if (len >= cap) {
                size_t newcap = cap ? cap << 1 : 4;
                if (newcap < cap + 1) newcap = cap + 1;
                seq_t *tmp = (seq_t *)realloc(seq,
                                               newcap * sizeof(seq_t));
                seq = tmp;
                cap = newcap;
            }
            seq[len++] = (seq_t)cand;
        }
        cand++;
    }

    return seq[n - 1];
}
