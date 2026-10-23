/*
 * A014754  Primes p == 1 mod 8 such that 2 and -2 are both 4th powers (one
 *          implies other) mod p.
 * https://oeis.org/A014754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 805, u32 n <= 9769, u64 n <= 9769,
 *            u128 n <= 9769, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A014754:
   Primes p ≡ 1 (mod 8) such that 2^((p-1)/4) ≡ 1 (mod p)  (i.e. 2 is a fourth power residue). */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"   /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/is_prime.h"  /* bool is_prime(idx_t); */

seq_t a(idx_t n)
{
    /* Handle already computed terms */
    static seq_t *seq_vals  = NULL;   /* sequence values */
    static size_t seq_cap   = 0;      /* current allocation */
    static size_t seq_len   = 0;      /* number of values stored */

    /* Next candidate prime to examine (prime ≡ 1 mod 8) */
    static idx_t next_candidate = 73;

    /* Ensure at least n terms are computed */
    while (seq_len < n)
    {
        /* Search candidate primes until the next qualifying prime is found */
        for (/* candidate starts at next_candidate */; ; next_candidate += 8)
        {
            if (!is_prime(next_candidate))
                continue;

            /* Check the fourth‑power residue condition */
            uint64_t exp = (next_candidate - 1ULL) / 4ULL;
            if (mod_pow((seq_t)2, exp, (seq_t)next_candidate) == (seq_t)1)
            {
                /* Store the found prime, resizing if necessary */
                if (seq_len == seq_cap)
                {
                    seq_cap = seq_cap ? seq_cap * 2 : 16;
                    seq_vals = (seq_t *)realloc(seq_vals, seq_cap * sizeof(seq_t));
                }
                seq_vals[seq_len++] = (seq_t)next_candidate;
                break; /* Found one more term */
            }
        }
        /* Advance candidate to the next number after the one we just used */
        next_candidate += 8;
    }

    /* Return the requested term (1‑based index) */
    return seq_vals[n - 1];
}
