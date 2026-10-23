/*
 * A140383  Prime triples of the form p, 2^p-1, 2^(2^p)+1 derived from a
 *          single prime p.
 * https://oeis.org/A140383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   is_prime, is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates the OEIS sequence A140383.
   The sequence consists of the numbers of the prime triples
   (p, 2^p−1, 2^(2^p)+1) for each prime p that yields all three
   numbers prime.  The terms are returned in the order
   p, 2^p−1, 2^(2^p)+1 for each qualifying prime p. */

/* Include the helper functions we need */
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/pow2.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t term_idx = 0;            /* current position in the sequence */

    /* Only small primes can produce such triples, so we check
       primes up to 7 (the largest 2^p that fits comfortably
       in a 128‑bit integer for the Fermat test). */
    for (idx_t p = 2; p <= 7; ++p) {
        if (!is_prime(p)) continue;            /* p must be prime */

        /* 2^p - 1 (the Mersenne number) */
        seq_t m = pow2(p);
        m -= 1;
        if (!is_prime_big(m)) continue;        /* must be prime */

        /* 2^(2^p) + 1 (the Fermat candidate).
           For p=7, 2^p = 128, which would need 129 bits; skip it. */
        idx_t ferm_exp = ((idx_t)1 << p);      /* 2^p as idx_t */
        if (ferm_exp > 127) continue;          /* cannot fit */
        seq_t f = pow2(ferm_exp);
        f += 1;
        if (!is_prime_big(f)) continue;        /* must be prime */

        /* Emit the three numbers in order. */
        if (++term_idx == n) return (seq_t)p;
        if (++term_idx == n) return m;
        if (++term_idx == n) return f;
    }

    /* If the sequence ends before reaching the requested index,
       return 0 (no further terms are known). */
    return 0;
}
