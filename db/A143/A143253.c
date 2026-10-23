/*
 * A143253  Irregular triangle by rows, squares mod primes; 1<=k<=n.
 * https://oeis.org/A143253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   * a(n) is obtained by concatenating rows of the “irregular
 *            triangle”. * Row m uses the m‑th prime p = p(m); it contains the
 *            residues * of k² modulo p for k = 1,2,…,p−1. The sequence is
 *            read row by * row, so a(n) is the n‑th term in this flattening.
 *            * (Equivalent to: find the row m such that * Σ_{...
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *primes = NULL;        /* list of primes p(n)     */
static uint64_t *prefix = NULL;        /* prefix sums of (p-1)   */
static size_t   primes_cap   = 0;      /* allocated capacity     */
static size_t   primes_len   = 0;      /* number of primes stored*/

static void ensure_capacity(size_t needed_cap)
{
    if (needed_cap <= primes_cap) return;
    size_t new_cap = primes_cap ? primes_cap << 1 : 16;
    while (new_cap < needed_cap) new_cap <<= 1;
    uint64_t *new_primes = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    uint64_t *new_prefix = (uint64_t *)realloc(prefix, new_cap * sizeof(uint64_t));
    primes   = new_primes;
    prefix   = new_prefix;
    primes_cap = new_cap;
}

static void add_prime(uint64_t p)
{
    ensure_capacity(primes_len + 1);
    primes[primes_len] = p;
    uint64_t last_prefix = (primes_len == 0) ? 0 : prefix[primes_len - 1];
    prefix[primes_len] = last_prefix + (p - 1);
    primes_len += 1;
}

static bool is_prime_candidate(uint64_t cand)
{
    if (cand < 2) return false;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p * p > cand) break;
        if (cand % p == 0) return false;
    }
    return true;
}

static void extend_primes(idx_t needed_total_terms)
{
    /* Extend prime list until cumulative length >= needed_total_terms. */
    if (primes_len == 0) add_prime(2);       /* prime #1 */
    while (prefix[primes_len - 1] < (uint64_t)needed_total_terms) {
        uint64_t next = primes[primes_len - 1] + 1;
        while (!is_prime_candidate(next)) next += 1;
        add_prime(next);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, guard */
    extend_primes(n);

    /* Find the row r (0‑based) containing term n. */
    size_t r = 0;
    while (prefix[r] < (uint64_t)n) ++r;          /* r is safe because we extended above */

    uint64_t prev_sum = (r == 0) ? 0 : prefix[r - 1];
    uint64_t pos_in_row = (uint64_t)n - prev_sum - 1; /* 0‑based */
    uint64_t k = pos_in_row + 1;                    /* k ranges 1 .. p-1 */
    uint64_t p = primes[r];
    uint64_t prod = k * k;                          /* fits in 64‑bit for n up to ~10k */
    seq_t result = (seq_t)(prod % p);
    return result;
}
