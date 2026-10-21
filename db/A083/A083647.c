/*
 * A083647  For primes p: Number of steps to reach 2 when iterating f(p) =
 *          greatest prime divisor of p-1.
 * https://oeis.org/A083647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A083647: for the n-th prime p, the number of steps to reach 2
   by repeatedly applying f(x) = largest prime factor of (x–1).
   a(1) = 0  (p = 2)   a(2) = 1  (p = 3)   etc.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gpf.h"   /* has uint64_t gpf(uint64_t x) */

/*--- static storage for the prime table -----------------------------------*/
static uint64_t *prime_table = NULL;   /* list of primes in ascending order */
static size_t   prime_cnt   = 0;        /* number of primes stored       */
static size_t   prime_cap   = 0;        /* capacity of prime_table       */

/* Ensure that at least 'need' primes are in prime_table.  */
static void ensure_primes(idx_t need)
{
    if ((size_t)need <= prime_cnt)
        return;                       /* already available                */

    /* enlarge storage if needed */
    if (!prime_table) {
        prime_cap = (size_t)need > 32 ? need : 32;
        prime_table = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_table) exit(1);
        /* initialise with the first prime 2 */
        prime_table[0] = 2;
        prime_cnt = 1;
    } else if (prime_cnt + ((size_t)need - prime_cnt) > prime_cap) {
        prime_cap = (prime_cap * 3) / 2 + (size_t)need;
        uint64_t *tmp = (uint64_t *)realloc(prime_table, prime_cap * sizeof(uint64_t));
        if (!tmp) exit(1);
        prime_table = tmp;
    }

    /* generate further primes by trial division using the current list   */
    uint64_t candidate = prime_table[prime_cnt - 1] + 1;
    if (candidate <= 2) candidate = 3;
    if ((candidate & 1) == 0) ++candidate;   /* start with an odd number */

    while (prime_cnt < (size_t)need) {
        bool prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_table[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { prime = false; break; }
        }
        if (prime) {
            prime_table[prime_cnt++] = candidate;
        }
        candidate += 2;   /* skip even numbers */
    }
}

/*--------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n = 0, guard    */

    /* fetch the n-th prime (1‑based) */
    ensure_primes(n);
    uint64_t p = prime_table[(size_t)n - 1];

    /* iterate gpf until we hit 2 and count steps */
    seq_t steps = 0;
    uint64_t current = p;
    while (current != 2) {
        current = gpf(current - 1);
        ++steps;
    }
    return steps;
}
