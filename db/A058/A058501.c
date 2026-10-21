/*
 * A058501  Primes p such that largest odd factor of p-1 is not a prime (i.e.,
 *          is composite or 1).
 * https://oeis.org/A058501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 5497, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A058501: primes p such that the largest odd factor of p-1 is not prime
   (i.e., is composite or 1).  The function generates primes incrementally,
   keeps those that satisfy the condition, and returns the n‑th such prime
   (1‑based).  The algorithm uses trial division for primality checking
   and grows the storage arrays on demand.  It works for any seq_t /
   idx_t supported by the build. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Growth factors for the dynamic arrays */
#define PRIME_GROWTH_FACTOR 2
#define SEQ_GROWTH_FACTOR   2

/* Static storage for primes and the sequence */
static uint64_t next_candidate = 2;     /* next number to test for primality    */
static uint64_t *prime_list   = NULL;   /* all primes discovered so far */
static size_t   prime_cap    = 0;
static size_t   prime_cnt    = 0;

static uint64_t *seq_list     = NULL;   /* primes satisfying the condition */
static size_t   seq_cap       = 0;
static size_t   seq_cnt       = 0;

/* Ensure prime_list has space for at least min_cap elements */
static void grow_prime_cap(size_t min_cap)
{
    if (prime_cap >= min_cap) return;
    size_t new_cap = prime_cap ? prime_cap * PRIME_GROWTH_FACTOR : min_cap;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* Ensure seq_list has space for at least min_cap elements */
static void grow_seq_cap(size_t min_cap)
{
    if (seq_cap >= min_cap) return;
    size_t new_cap = seq_cap ? seq_cap * SEQ_GROWTH_FACTOR : min_cap;
    uint64_t *tmp = (uint64_t *)realloc(seq_list, new_cap * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);
    seq_list = tmp;
    seq_cap  = new_cap;
}

/* Small primality test using the primes already found */
static bool is_prime_u64(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Produce terms until the sequence has at least n elements */
static void ensure_terms(size_t n)
{
    while (seq_cnt < n) {
        uint64_t p = next_candidate;

        bool prime_flag = false;
        if (p == 2) {
            prime_flag = true;
        } else if ((p & 1ULL) == 0) {
            prime_flag = false;
        } else {
            prime_flag = true;
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint64_t q = prime_list[i];
                if (q * q > p) break;
                if (p % q == 0) { prime_flag = false; break; }
            }
        }

        if (prime_flag) {
            /* Record the new prime */
            grow_prime_cap(prime_cnt + 1);
            prime_list[prime_cnt++] = p;

            /* Largest odd factor of p-1 */
            uint64_t m = p - 1;
            unsigned int tz = __builtin_ctzll(m);
            uint64_t odd = m >> tz;   /* remove powers of two */

            /* Qualify if odd part is 1 or not prime */
            bool qualifies = (odd == 1) || !is_prime_u64(odd);

            if (qualifies) {
                grow_seq_cap(seq_cnt + 1);
                seq_list[seq_cnt++] = p;
            }
        }

        /* Advance candidate */
        if (next_candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;     /* skip even numbers > 2 */
    }
}

/* Required outward function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset 1 – safe behaviour for n<1 */
    ensure_terms((size_t)n);
    return (seq_t)seq_list[n - 1];
}
