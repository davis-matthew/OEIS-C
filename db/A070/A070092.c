/*
 * A070092  Number of isosceles integer triangles with perimeter n and prime
 *          side lengths.
 * https://oeis.org/A070092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   simple_is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A070092
   Number of isosceles integer triangles with perimeter n and prime side lengths.
   Computation: For each prime p with n/4 < p < n/2, set q = n - 2p.  If q is prime
   and the triangle inequality is satisfied (which it automatically is for this
   interval), count the triangle.  The result is the number of such triangles. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/simple_is_prime.h"

/* Static state for sieve */
static bool *is_prime_tbl = NULL;
static idx_t sieve_limit = 0;
static idx_t *prime_list = NULL;
static size_t prime_count = 0;

/* Allocate/resize sieve up to 'limit', store primes up to 'limit' */
static void compute_sieve(idx_t limit)
{
    size_t sz = (size_t)limit + 1;
    if (is_prime_tbl) free(is_prime_tbl);
    is_prime_tbl = (bool *)malloc(sz * sizeof(bool));
    if (!is_prime_tbl) return;          /* allocation failure - unlikely */

    for (size_t i = 0; i < sz; ++i) is_prime_tbl[i] = true;
    if (limit >= 0) is_prime_tbl[0] = false;
    if (limit >= 1) is_prime_tbl[1] = false;

    for (size_t i = 2; i * i <= (size_t)limit; ++i) {
        if (is_prime_tbl[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                is_prime_tbl[j] = false;
        }
    }

    free(prime_list);
    /* Rough upper bound for number of primes ≤ limit: limit/10 + 100 is safe for small limit */
    size_t alloc = limit / 10 + 100;
    prime_list = (idx_t *)malloc(alloc * sizeof(idx_t));
    if (!prime_list) { prime_count = 0; return; }
    prime_count = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i) {
        if (is_prime_tbl[i]) {
            if (prime_count == alloc) {            /* grow if needed */
                alloc *= 2;
                prime_list = (idx_t *)realloc(prime_list, alloc * sizeof(idx_t));
            }
            prime_list[prime_count++] = (idx_t)i;
        }
    }
    sieve_limit = limit;
}

/* OEIS sequence A070092 */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Ensure sieve covers up to at least n */
    if (n > sieve_limit) compute_sieve(n);

    seq_t count = 0;

    /* p is the repeated side.  Must satisfy n/4 < p < n/2 to keep triangle inequality. */
    idx_t p_start = n / 4 + 1;          /* floor(n/4) + 1 */
    idx_t p_end   = n / 2;              /* exclusive upper bound */

    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p >= p_end) break;
        if (p < p_start) continue;

        idx_t q = n - 2 * p;            /* third side */
        if (q <= sieve_limit) {
            if (is_prime_tbl[q]) ++count;
        } else {                        /* should not happen for q ≤ n/2 */
            if (simple_is_prime(q)) ++count;
        }
    }

    return count;
}
