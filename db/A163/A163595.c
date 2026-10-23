/*
 * A163595  Numbers k such that prime(k) == 5 (mod 9).
 * https://oeis.org/A163595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer k such that the k‑th prime is congruent to
   5 modulo 9.  The sequence starts 3, 9, 13, 17, … (OEIS A163595). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Static state for incremental prime generation */
static uint64_t *prime_list = NULL;   /* Array of discovered primes   */
static size_t    prime_cnt  = 0;      /* Number of primes in prime_list */
static size_t    prime_cap  = 0;      /* Allocated capacity of prime_list */

static idx_t *qual_k = NULL;          /* Indices k with prime(k) % 9 == 5 */
static size_t  qual_cnt = 0;          /* Number of such indices stored */
static size_t  qual_cap = 0;          /* Allocated capacity of qual_k */

static uint64_t next_candidate = 2;   /* Next integer to test for primality */

/* Ensure the array @ptr has space for @needed elements. */
static void grow_array(void **ptr, size_t *cap, size_t needed, size_t elem_size)
{
    if (needed > *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        if (new_cap < needed) new_cap = needed;
        void *tmp = realloc(*ptr, new_cap * elem_size);
        if (!tmp) abort();
        *ptr = tmp;
        *cap = new_cap;
    }
}

/* Simple primality test using previously found primes. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Generate primes and qualifying indices until we have at least @need qualifiers. */
static void ensure_n(idx_t need)
{
    if (qual_cnt >= (size_t)need) return;      /* already have enough */

    while (qual_cnt < (size_t)need) {
        /* Find next prime */
        uint64_t cand = next_candidate++;
        if (is_prime_uint64(cand)) {
            /* Append to prime list */
            grow_array((void**)&prime_list, &prime_cap, prime_cnt + 1, sizeof(uint64_t));
            prime_list[prime_cnt++] = cand;

            /* Check congruence condition */
            if (cand % 9 == 5) {
                grow_array((void**)&qual_k, &qual_cap, qual_cnt + 1, sizeof(idx_t));
                qual_k[qual_cnt++] = prime_cnt;   /* prime_cnt is the index k */
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; n=0 is undefined */
    ensure_n(n);
    return (seq_t)qual_k[n - 1];
}
