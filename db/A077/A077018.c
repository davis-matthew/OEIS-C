/*
 * A077018  Closest prime to n (break ties by taking the larger prime).
 * https://oeis.org/A077018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For each integer n (with offset 0) return the prime that is
   closest to n; if two primes are equally close, choose the larger one. */

/* Include required standard headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*--- Prime generation infrastructure ----------------------------------------*/
static uint64_t *prime_list = NULL;   /* dynamic array of discovered primes */
static size_t   prime_cnt   = 0;       /* number of primes stored          */
static size_t   prime_cap   = 0;       /* capacity of the array            */
static uint64_t max_checked = 1;      /* all integers up to this have
                                      * been examined for primality and stored
                                      * as primes if applicable                 */

/* Append a new prime to the dynamic array, enlarging if necessary. */
static void push_prime(uint64_t p)
{
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) abort();          /* allocation failure – terminate */
        prime_list = tmp;
        prime_cap = new_cap;
    }
    prime_list[prime_cnt++] = p;
}

/* Extend the prime list up to (and including) the given bound.
   Only numbers greater than the current maximum are examined. */
static void extend_primes(uint64_t up_to)
{
    if (up_to <= max_checked) return;

    for (uint64_t cand = max_checked + 1; cand <= up_to; ++cand) {
        if (cand < 2) continue;       /* ignore numbers < 2 */
        bool is_prime = true;

        /* Trial division by known primes up to sqrt(cand). */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p > cand / p) break;          /* p > sqrt(cand) */
            if (cand % p == 0) { is_prime = false; break; }
        }

        if (is_prime) push_prime(cand);
    }
    max_checked = up_to;
}

/* Test whether x is prime using the current prime table.
   The table is automatically enlarged to at least x if necessary. */
static bool is_prime_u64(uint64_t x)
{
    if (x < 2) return false;
    extend_primes(x);            /* ensure primes up to x are known */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > x / p) break;    /* p > sqrt(x) */
        if (x % p == 0) return false;
    }
    return true;
}

/*--- OEIS sequence A077018 --------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Handle the easy small cases explicitly. */
    if (n <= 2) return (seq_t)2;   /* a(0)=2, a(1)=2, a(2)=2 */

    uint64_t target = (uint64_t)n;

    /* Search downward for the closest lower or equal prime. */
    uint64_t lower = target;
    while (lower >= 2 && !is_prime_u64(lower)) {
        --lower;
    }

    /* Search upward for the closest higher or equal prime. */
    uint64_t upper = target;
    while (!is_prime_u64(upper)) {
        ++upper;
    }

    /* Compute distances and pick according to the tie rule. */
    uint64_t dist_lower = target - lower;
    uint64_t dist_upper = upper - target;

    if (dist_lower < dist_upper) {
        return (seq_t)lower;
    } else {
        return (seq_t)upper;      /* ties resolve to larger prime */
    }
}
