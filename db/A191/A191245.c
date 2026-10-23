/*
 * A191245  Primes that remain prime if the bit pattern 10 is attached as the
 *          most-significant bits to their binary representation.
 * https://oeis.org/A191245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   bitlen_seq_t, is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A191245
 *  A( n ) = the n‑th prime p such that p + 2^( ⌊log2 p⌋ + 1 ) is prime.
 *  The sequence is computed by generating primes in order and testing
 *  the shifted number with the provided big‑integer primality routine.
 *  Results are memoised so that calls in arbitrary order run in
 *  O( max requested n ) time.  */
/*  This implementation uses only the utilities supplied by the
 *  framework:  is_prime_big, pow2, bitlen_seq_t, and standard C
 *  allocation functions.  */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime_big.h"   /* bool is_prime_big( seq_t n );        */
#include "../utils/pow2.h"           /* seq_t pow2( idx_t exp );            */
#include "../utils/bitlen_seq_t.h"   /* unsigned bitlen_seq_t( seq_t x );   */

/* ------------------------------------------------------------ */
/*  Prime generation (uint64_t) – keeps a small dynamic array   */
/* ------------------------------------------------------------ */
static uint64_t *prime_list      = NULL;   /* primes found so far   */
static size_t   prime_cap       = 0;       /* capacity of prime_list*/
static size_t   prime_cnt       = 0;       /* number of stored primes*/
static uint64_t sieve_next_candidate = 2;  /* next integer to test  */

static uint64_t get_next_prime(void)
{
    while (1) {
        uint64_t n = sieve_next_candidate;
        if (n < 2) { ++sieve_next_candidate; continue; }

        bool prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > n) break;
            if (n % p == 0) { prime = false; break; }
        }

        if (prime) {
            /* expand storage if necessary */
            if (prime_cnt == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap << 1 : 16;
                uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                    new_cap * sizeof(uint64_t));
                if (!tmp) exit(1);
                prime_list = tmp;
                prime_cap  = new_cap;
            }
            prime_list[prime_cnt++] = n;
            ++sieve_next_candidate;
            return n;
        }
        ++sieve_next_candidate;
    }
}

/* ------------------------------------------------------------ */
/*  Sequence cache                                               */
/* ------------------------------------------------------------ */
static seq_t  *seq_cache = NULL;    /* cached sequence values         */
static size_t seq_cap   = 0;        /* capacity of seq_cache          */
static idx_t  seq_len   = 0;        /* number of filled elements      */

/* ------------------------------------------------------------ */
/*  Main sequence function                                      */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset 1 – undefined for n < 1 */

    /* initialise cache if required */
    if (!seq_cache) {
        seq_cap   = 64;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_cache) exit(1);
    }

    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        uint64_t p_uint = get_next_prime();          /* next prime */
        seq_t   p       = (seq_t)p_uint;            /* cast to seq_t */

        /* compute shift = 2^(bits+1) where bits = floor(log2 p) + 1 */
        unsigned bits   = bitlen_seq_t(p);
        seq_t   shift   = pow2((idx_t)(bits + 1));
        seq_t   q       = p + shift;                 /* shifted number */

        if (is_prime_big(q)) {                       /* test primality */
            /* grow cache if needed */
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(seq_cache,
                                              seq_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                seq_cache = tmp;
            }
            seq_cache[seq_len++] = p;                /* store valid prime */
        }
    }

    return seq_cache[n - 1];
}
