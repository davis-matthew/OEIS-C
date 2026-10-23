/*
 * A176180  Primes p such that gcd(p(n)-1, p(n+1)-1) != gcd(p(n)+1, p(n+1)+1).
 * https://oeis.org/A176180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A176180: primes p such that gcd(p-1,q-1) != gcd(p+1,q+1) where q is the next prime after p. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* static storage for all primes found */
static uint64_t *prime_nums = NULL;
static size_t    prime_len  = 0;
static size_t    prime_cap  = 0;

/* static storage for qualifying primes */
static seq_t *qual_primes = NULL;
static size_t   qual_len  = 0;
static size_t   qual_cap  = 0;

/* enlarge prime array when needed */
static void grow_prime_array(void)
{
    if (prime_len >= prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(prime_nums, new_cap * sizeof(uint64_t));
        prime_nums = tmp;
        prime_cap  = new_cap;
    }
}

/* enlarge qualifying prime array when needed */
static void grow_qual_array(void)
{
    if (qual_len >= qual_cap) {
        size_t new_cap = qual_cap ? qual_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(qual_primes, new_cap * sizeof(seq_t));
        qual_primes = tmp;
        qual_cap    = new_cap;
    }
}

/* primality test using already found primes (trial division) */
static bool is_prime_candidate(uint64_t x)
{
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_nums[i];
        if (p > x / p)
            break;
        if (x % p == 0)
            return false;
    }
    return true;
}

/* generate the next prime and, if it satisfies the condition, record the previous prime */
static void generate_next_prime(void)
{
    uint64_t candidate;

    if (prime_len == 0) {
        candidate = 2;
    } else {
        candidate = prime_nums[prime_len - 1] + 1;
        if ((candidate & 1) == 0)
            candidate++;               /* skip even numbers after 2 */
    }

    for (;;) {
        if (is_prime_candidate(candidate)) {
            /* test the condition with the previous prime */
            if (prime_len > 0) {
                seq_t prev = (seq_t)prime_nums[prime_len - 1];
                seq_t curr = (seq_t)candidate;
                seq_t g1 = gcd(prev - 1, curr - 1);
                seq_t g2 = gcd(prev + 1, curr + 1);
                if (g1 != g2) {
                    grow_qual_array();
                    qual_primes[qual_len++] = prev;
                }
            }
            grow_prime_array();
            prime_nums[prime_len++] = candidate;
            break;
        }
        candidate += 2; /* only test odd candidates after 2 */
    }
}

/* public function required by the interface */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for offset 0 */

    while (qual_len < (size_t)n)
        generate_next_prime();

    return qual_primes[(size_t)n - 1];
}
