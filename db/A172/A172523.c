/*
 * A172523  Partial sums of primes in which no digit is a prime A061372.
 * https://oeis.org/A172523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   is_holey_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A172523 – partial sums of primes whose decimal digits are
    0,4,6,8,9 (so‑called “holey primes”).  The function computes
    a(n) = Σ_{i=1..n} h_i   where h_i is the i‑th holey prime.  The
    implementation memoises the partial sums and the primes found
    so that repeated calls are incremental.  It uses trial division
    by previously found primes and expands the buffers dynamically. */
     
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_holey_prime.h"

/* --------------------------------------------------------------------- */
/*                           static state                                 */
/* --------------------------------------------------------------------- */

/* list of primes discovered so far                                         */
static uint64_t *prime_list = NULL;
static size_t   prime_cap   = 0;
static size_t   prime_cnt   = 0;

/* next odd number to test for primality                                  */
static uint64_t next_candidate = 3;

/* cumulative sums of holey primes                                          */
static seq_t *cumsum      = NULL;
static size_t cumsum_cap  = 0;
static idx_t  cumsum_cnt  = 0;   /* 0 means no sums stored yet          */
static seq_t  current_sum = 0;   /* running total of holey primes        */

/* --------------------------------------------------------------------- */
/*                      helper predicates                                 */
/* --------------------------------------------------------------------- */

/* trial‑division primality test using previously found primes             */
static inline bool is_prime(uint64_t n) {
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > n / p)   /* avoid overflow of p*p */
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/*                      initialise buffers                               */
/* --------------------------------------------------------------------- */

static void init(void) {
    if (prime_list != NULL)
        return;   /* already initialised */

    /* prime list – seed with 2                                       */
    prime_cap  = 64;
    prime_list = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
    prime_list[prime_cnt++] = 2;

    /* cumulative sums – 0 is a sentinel so a(0) would be 0            */
    cumsum_cap  = 64;
    cumsum      = (seq_t*)malloc(cumsum_cap * sizeof(seq_t));
    cumsum[0]   = 0;
    cumsum_cnt  = 0;
    current_sum = 0;
}

/* --------------------------------------------------------------------- */
/*                    public function a(idx_t n)                          */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n) {
    if (n < 1) return 0;  /* undefined for n<1 – be permissive            */

    init();

    /* grow the sequence until we have at least n terms                */
    while (cumsum_cnt < n) {
        /* find the next prime number                                 */
        uint64_t p = 0;
        for (;;) {
            if (is_prime(next_candidate)) {
                p = next_candidate;
                break;
            }
            next_candidate += 2;    /* skip even numbers                */
        }

        /* store prime for future tests                               */
        if (prime_cnt == prime_cap) {
            prime_cap <<= 1;        /* double the capacity               */
            prime_list = (uint64_t*)realloc(prime_list,
                                            prime_cap * sizeof(uint64_t));
        }
        prime_list[prime_cnt++] = p;
        next_candidate += 2;          /* advance to the following odd     */

        /* update cumulative sum if the prime is a holey prime      */
        if (is_holey_prime(p)) {
            current_sum += (seq_t)p;

            /* ensure enough space for the new entry                 */
            if ((size_t)(cumsum_cnt + 1) >= cumsum_cap) {
                cumsum_cap <<= 1;
                cumsum = (seq_t*)realloc(cumsum,
                                         cumsum_cap * sizeof(seq_t));
            }
            cumsum[++cumsum_cnt] = current_sum;
        }
    }

    return cumsum[n];
}
