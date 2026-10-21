/*
 * A075465  Rounded average of first n primes.
 * https://oeis.org/A075465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = round(A007504(n) / n), where A007504(n) is the sum of the
 *            first n primes. Ties (average exactly equal to an integer + 0.5)
 *            are rounded down
 * Range:     u8 n <= 13, u16 n <= 858, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static state ----------------------------------------------------------- */
static idx_t *prime_list = NULL;   /* primes themselves, indices 0..prime_count-1   */
static seq_t *prefix_sums = NULL;  /* prefix sums of primes                      */
static size_t prime_count = 0;     /* how many primes we have                    */
static size_t prime_cap   = 0;     /* allocated capacity                         */
static idx_t  next_candidate = 2;  /* next integer to test for primality         */

/* ensure storage can hold at least 'needed' entries ---------------------- */
static void ensure_capacity(size_t needed)
{
    if (needed <= prime_cap) return;
    size_t new_cap = prime_cap ? (prime_cap << 1) : 128;
    while (new_cap < needed) new_cap <<= 1;
    prime_list   = (idx_t*)realloc(prime_list  , new_cap * sizeof(idx_t));
    prefix_sums  = (seq_t*)realloc(prefix_sums, new_cap * sizeof(seq_t));
    prime_cap    = new_cap;
}

/* generate primes until we have at least 'target' primes */
static void generate_next_primes(size_t target)
{
    ensure_capacity(target);
    while (prime_count < target) {
        idx_t cand = next_candidate;
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            idx_t p = prime_list[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            prime_list[prime_count] = cand;
            prefix_sums[prime_count] = (prime_count == 0) ? cand
                                                          : prefix_sums[prime_count-1] + cand;
            ++prime_count;
        }
        ++next_candidate;
    }
}

/* main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                         /* safety, offset 1 */
    generate_next_primes((size_t)n);              /* ensure primes & sums */
    seq_t sum = prefix_sums[n-1];                 /* sum of first n primes */
    seq_t r   = sum % n;                          /* remainder after division */
    seq_t quo = sum / n;                          /* integer part of the average */
    if ((seq_t)2 * r > (seq_t)n) ++quo;           /* round up if fractional > 0.5 */
    /* ties (2*r == n) automatically stay rounded down          */
    return quo;
}
