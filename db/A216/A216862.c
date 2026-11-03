/*
 * A216862  Primes p such that x^5 = 20 has a solution mod p.
 * https://oeis.org/A216862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 938 (2026-11-03)
 * Helpers:   is_prime, mem, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Condition: prime p is included if p = 2 or 3 or 5,
   or p ≢ 1 (mod 5), or 20^((p‑1)/5) ≡ 1 (mod p).
   This is equivalent to: x^5 ≡ 20 has a solution modulo p. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/powmod_u64.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* create on first call */
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0, prime_cap = 0;
    static uint64_t next_candidate = 2;

    static seq_t *good_list = NULL;
    static size_t good_cnt = 0, good_cap = 0;

    if ( n == 0 )
        return (seq_t)0;          /* undefined for n < 1 */

    /* generate good primes until we have at least n of them */
    while ( good_cnt < (size_t)n )
    {
        /* generate next prime candidate */
        uint64_t cand = next_candidate;

        /* test primality */
        if ( is_prime( cand ) )
        {
            /* store prime */
            if ( prime_cap == 0 )
            {
                prime_cap = 8;
                prime_list = (uint64_t *)malloc( prime_cap * sizeof( uint64_t ) );
            }
            else if ( prime_cnt == prime_cap )
            {
                prime_cap <<= 1;
                prime_list = (uint64_t *)realloc( prime_list, prime_cap * sizeof( uint64_t ) );
            }
            prime_list[ prime_cnt++ ] = cand;

            /* decide whether this prime belongs to the sequence */
            bool good = false;
            if ( cand == 2 || cand == 3 || cand == 5 )
                good = true;
            else if ( cand % 5 != 1 )
                good = true;
            else
            {
                uint64_t base = 20 % cand;
                uint64_t exp  = ( cand - 1 ) / 5U;
                uint64_t r    = powmod_u64( base, exp, cand );
                good = ( r == 1U );
            }

            if ( good )
            {
                if ( good_cap == 0 )
                {
                    good_cap = 8;
                    good_list = mem_new(seq_t, good_cap);
                }
                else if ( good_cnt == good_cap )
                {
                    good_cap <<= 1;
                    good_list = mem_resize(seq_t, good_list, good_cap);
                }
                good_list[ good_cnt++ ] = (seq_t)cand;
            }
        }

        /* advance to next candidate number */
        if ( cand == 2 )
            next_candidate = 3;
        else
            next_candidate += 2;   /* skip even numbers after 2 */
    }

    return good_list[ n-1 ];
}
