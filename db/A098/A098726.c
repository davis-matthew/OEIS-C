/*
 * A098726  Take three consecutive primes starting with the n-th prime.
 *          Calculate d(i,j) = abs(prime(i) - prime(j)), for all {i,j}, i.e.,
 *          all possible differences. a(n) is the number of distinct
 *          differences (which can be either 3 or 2).
 * https://oeis.org/A098726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A098726: a(n) = 2 if the consecutive prime gaps between
   the n-th, (n+1)-st and (n+2)-nd primes are equal, otherwise 3. */

#include <stdlib.h>
#include <stdbool.h>

/* Prime cache */
static idx_t  prime_cnt  = 0;          /* number of primes stored   */
static size_t prime_cap  = 0;          /* allocated capacity       */
static uint64_t *prime_arr = NULL;     /* array of primes          */
static uint64_t next_candidate = 2;    /* next number to test      */

/* Append a new prime to the cache, growing the array if necessary */
static void add_prime(uint64_t p)
{
    if (prime_cnt == prime_cap) {
        size_t new_cap = (prime_cap == 0) ? 8 : prime_cap * 2;
        uint64_t *tmp = (uint64_t *)realloc(prime_arr, new_cap * sizeof(uint64_t));
        prime_arr = tmp;
        prime_cap = new_cap;
    }
    prime_arr[prime_cnt++] = p;
}

/* Generate primes until we have at least 'need' primes (indices 0..need-1) */
static void ensure_primes_up_to_index(idx_t need)
{
    if (prime_cnt >= need) return;              /* already enough */

    if (prime_cnt == 0) {                      /* initialise */
        add_prime(2);
        next_candidate = 3;
    }

    while (prime_cnt < need) {
        uint64_t cand = next_candidate;
        bool is_prime = true;

        /* trial division using stored primes */
        for (idx_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_arr[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }

        if (is_prime) add_prime(cand);

        next_candidate = cand + 1;             /* advance for next test */
    }
}

/* Public entry point for the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined, but harmless */

    idx_t need = n + 2;                          /* indices 0 .. n+1 */
    ensure_primes_up_to_index(need);

    seq_t p0 = (seq_t)prime_arr[n   - 1];
    seq_t p1 = (seq_t)prime_arr[n   ];
    seq_t p2 = (seq_t)prime_arr[n   + 1];

    seq_t g1 = p1 - p0;
    seq_t g2 = p2 - p1;

    return (g1 == g2) ? (seq_t)2 : (seq_t)3;
}
