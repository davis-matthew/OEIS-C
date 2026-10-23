/*
 * A175036  a(n) = 2^(n-1) mod prime(n).
 * https://oeis.org/A175036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) mod prime(n)
 * Range:     u8 n <= 54, u16 n <= 121, u32 n <= 121, u64 n <= 121,
 *            u128 n <= 121, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A175036.  */
/* Implements a lazy prime generator and modular exponentiation
   using only the permitted C operators. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

/* ----------------------------------------------------------- */
/*  Prime generation (lazy, incremental, 1‑based indexing)    */
/* ----------------------------------------------------------- */
static uint32_t *prime_list      = NULL;   /* array of generated primes     */
static size_t  prime_cnt        = 0;      /* how many primes are stored    */
static size_t  prime_alloc      = 0;      /* allocated capacity             */
static uint32_t next_candidate  = 3;      /* next odd number to test       */

static void ensure_primes(idx_t n)
{
    if (prime_cnt >= n)
        return;

    /* initialise the prime list */
    if (prime_cnt == 0) {
        prime_alloc = 32;
        prime_list = (uint32_t *)malloc(prime_alloc * sizeof(uint32_t));
        prime_list[prime_cnt++] = 2;
    }

    while (prime_cnt < n) {
        bool prime = true;
        uint32_t limit = (uint32_t)floor(sqrt((double)next_candidate));

        for (size_t i = 0; i < prime_cnt && prime_list[i] <= limit; ++i) {
            if (next_candidate % prime_list[i] == 0) {
                prime = false;
                break;
            }
        }

        if (prime) {
            if (prime_cnt == prime_alloc) {
                prime_alloc <<= 1;
                prime_list = (uint32_t *)realloc(prime_list,
                                    prime_alloc * sizeof(uint32_t));
            }
            prime_list[prime_cnt++] = next_candidate;
        }

        next_candidate += 2;          /* skip even numbers */
    }
}

/* ----------------------------------------------------------- */
/*                Sequence a(n)                                 */
/* ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for n < 1, but return 0 */
        return 0;

    ensure_primes(n);
    seq_t p       = (seq_t)prime_list[n - 1];    /* prime(n)            */
    seq_t exp     = (seq_t)(n - 1);             /* exponent n-1        */
    seq_t result  = 1 % p;
    seq_t base    = 2 % p;

    while (exp) {
        if (exp & 1) {
            uint64_t prod = (uint64_t)result * (uint64_t)base;
            result = (seq_t)(prod % p);
        }
        uint64_t prod2 = (uint64_t)base * (uint64_t)base;
        base = (seq_t)(prod2 % p);
        exp >>= 1;
    }
    return result;
}
