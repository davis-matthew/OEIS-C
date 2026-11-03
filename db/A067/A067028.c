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
 * Validated: standard, pass, 70 terms through n = 973 (2026-11-03)
 * Helpers:   bigomega_seq, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A067028: numbers with a composite number of prime factors
 * (counted with multiplicity).  Computed by sequential search. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/bigomega_seq.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Return true if x has a composite big‑omega value. */
static inline bool composite_omega(uint64_t x)
{
    if (x <= 1) return false;          /* Ω(1) = 0, not composite */
    seq_t omega = bigomega_seq((seq_t)x);
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
                seq_t *tmp = mem_resize(seq_t, seq, newcap);
                seq = tmp;
                cap = newcap;
            }
            seq[len++] = (seq_t)cand;
        }
        cand++;
    }

    return seq[n - 1];
}
