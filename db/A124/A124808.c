/*
 * A124808  Number of numbers k <= n such that k^2 + 1 is squarefree.
 * https://oeis.org/A124808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 356, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A124808
   a(n) = number of k <= n such that k^2 + 1 is squarefree.
   a(0) = 1
*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *prefix      = NULL;      /* prefix[k] = a(k)            */
static idx_t  prefix_len  = 0;          /* indices 0 .. prefix_len-1   */

static uint64_t *prime_list = NULL;    /* primes up to current_limit   */
static size_t   prime_cnt  = 0;         /* current number of primes     */
static size_t   prime_cap  = 0;         /* allocated capacity           */
static idx_t    prime_limit = 0;        /* largest integer tested       */

/* Ensure we have primes up to 'limit' (>=2).  1 is not added. */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;

    idx_t start = prime_limit + 1;
    if (start < 2) start = 2;

    for (idx_t n = start; n <= limit; ++n) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > n) break;
            if (n % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap << 1 : 16;
                prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_cnt++] = (uint64_t)n;
        }
    }
    prime_limit = limit;
}

/* Check whether k^2 + 1 is squarefree. */
static bool is_squarefree_of(idx_t k)
{
    seq_t val = ((seq_t)k * (seq_t)k) + (seq_t)1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        seq_t pp = (seq_t)p * (seq_t)p;
        if (pp > val) break;
        if ((val % pp) == 0) return false;
    }
    return true;
}

/* Ensure prefix[0..n] is computed. */
static void ensure_prefix(idx_t n)
{
    if (n + 1 <= prefix_len) return;

    /* Extend the array. */
    prefix = (seq_t *)realloc(prefix, (n + 1) * sizeof(seq_t));

    /* Build primes up to at least n+1 for squarefree tests. */
    ensure_primes(n + 1);

    /* Compute new entries. */
    for (idx_t k = prefix_len; k <= n; ++k) {
        if (k == 0) {
            /* a(0) = 1 because 1 is squarefree. */
            prefix[0] = (seq_t)1;
        } else {
            seq_t inc = is_squarefree_of(k) ? (seq_t)1 : (seq_t)0;
            prefix[k] = prefix[k - 1] + inc;
        }
    }
    prefix_len = n + 1;
}

extern seq_t a(idx_t n)
{
    ensure_prefix(n);
    return prefix[n];
}
