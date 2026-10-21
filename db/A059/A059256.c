/*
 * A059256  Primes p such that x^29 = 2 has no solution mod p.
 * https://oeis.org/A059256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 229, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth prime p (n ≥ 1) such that the congruence
   x^29 ≡ 2 (mod p) has no solution.  For an odd prime p the congruence
   has a solution iff 2^((p-1)/gcd(29,p-1)) ≡ 1 (mod p).  Since 29 is
   prime this reduces to checking only primes p ≡ 1 (mod 29) and testing
   whether 2^((p-1)/29) ≠ 1 (mod p).  The function generates primes in
   ascending order, filters them, and caches the results so that calls
   of any order are fast. */

#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/mod_pow.h"

/* Cached sequence values */
static seq_t *seq     = NULL;
static size_t seq_len = 0;   /* number of terms stored          */
static size_t seq_cap = 0;   /* current allocated capacity       */

/* Last prime that was examined */
static uint64_t last_checked = 2;

/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Ensure we have at least n terms stored */
    while (seq_len < (size_t)n)
    {
        /* Find next prime > last_checked */
        last_checked = next_prime(last_checked + 1);

        /* Only primes ≡ 1 (mod 29) can lack a 29th‑root of 2 */
        if ((last_checked - 1ULL) % 29ULL == 0ULL)
        {
            uint64_t exp = (last_checked - 1ULL) / 29ULL;
            seq_t  res  = mod_pow( (seq_t)2, exp, (seq_t)last_checked );

            if (res != (seq_t)1)
            {
                /* Grow storage if needed */
                if (seq_len == seq_cap)
                {
                    size_t new_cap = seq_cap ? seq_cap * 2 : 4;
                    seq_t *new_ptr = (seq_t *)realloc( seq, new_cap * sizeof(seq_t) );
                    if (!new_ptr)
                    {
                        /* Allocation failure – clean up and abort */
                        free( seq );
                        seq = NULL;
                        seq_len = seq_cap = 0;
                        return (seq_t)0;
                    }
                    seq     = new_ptr;
                    seq_cap = new_cap;
                }
                seq[seq_len++] = (seq_t)last_checked;
            }
        }
    }

    return seq[n-1];
}
