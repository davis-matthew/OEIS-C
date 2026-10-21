/*
 * A215355  Primes congruent to {0, 2, 3, 4, 5} mod 13.
 * https://oeis.org/A215355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute primes congruent to {0,2,3,4,5} mod 13.  
    The function finds the n‑th such prime by generating primes 
    in ascending order and checking the residue.  */
/*  The sequence starts with a(1)=2, a(2)=3, …  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Static state retained between calls ---------------------------------- */
static uint64_t *prime_list = NULL;   /* primes found so far                  */
static size_t   prime_cnt  = 0;       /* number of primes in prime_list       */
static size_t   prime_cap  = 0;       /* allocated capacity of prime_list     */

static seq_t *seq_cache = NULL;       /* cached sequence terms                */
static size_t   seq_len   = 0;        /* number of terms cached               */
static size_t   seq_cap   = 0;        /* allocated capacity of seq_cache      */

static uint64_t next_candidate = 2;   /* next integer to test for primality   */

/*  Ensure there is room in the prime list ------------------------------ */
static void enlarge_primes(void)
{
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    uint64_t *tmp = (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) abort();                 /* out of memory */
    prime_list = tmp;
    prime_cap  = new_cap;
}

/*  Ensure there is room in the sequence cache ------------------------- */
static void enlarge_seq(void)
{
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* out of memory */
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/*  Test primality of a 64‑bit integer using the current prime list. ----- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/*  Sequence entry ------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* behaviour below offset is irrelevant; return 0 for n==0 */
    if (n == 0) return 0;

    /* use cache if already computed */
    if ((size_t)n <= seq_len)
        return seq_cache[(size_t)n - 1];

    /* generate terms until we reach the requested index */
    while (seq_len < (size_t)n) {
        uint64_t candidate = next_candidate;

        /* test for primality */
        bool prime = is_prime_uint64(candidate);
        if (prime) {
            /* add to prime list */
            if (prime_cnt == prime_cap) enlarge_primes();
            prime_list[prime_cnt++] = candidate;

            /* check residue condition */
            uint8_t r = candidate % 13;
            if (r == 0 || r == 2 || r == 3 || r == 4 || r == 5) {
                if (seq_len == seq_cap) enlarge_seq();
                seq_cache[seq_len++] = (seq_t)candidate;
                if (seq_len == (size_t)n) {
                    next_candidate = candidate + 1;
                    return seq_cache[(size_t)n - 1];
                }
            }
        }
        /* advance to next integer to test */
        next_candidate = candidate + 1;
    }

    /* unreachable but satisfies compiler */
    return seq_cache[(size_t)n - 1];
}
