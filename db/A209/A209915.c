/*
 * A209915  Number of ways the n-th prime p(n) can be written as a multiple of
 *          its reversal A004086(p) +/- a prime q < p(n).
 * https://oeis.org/A209915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   is_prime, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the definition of OEIS A209915:
   a(n) = number of ways the n-th prime p can be written as
           p = m·rev(p) ± q, where q is a prime < p */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/rev.h"

/* Static cache of the first primes (idx_t type). */
static idx_t *prime_list   = NULL;
static size_t prime_len   = 0;
static size_t prime_cap   = 0;

/* Ensure that we have at least `n` primes stored in `prime_list`. */
static void
ensure_n_primes(idx_t n)
{
    if (prime_len >= n) return;

    /* Grow the array if necessary. */
    if (prime_cap < n) {
        prime_cap = n;
        prime_list = (idx_t *)realloc(prime_list, prime_cap * sizeof(idx_t));
        if (!prime_list) abort();
    }

    idx_t candidate = 2;
    if (prime_len > 0) candidate = prime_list[prime_len - 1] + 1;

    while (prime_len < n) {
        /* Skip even numbers after 2. */
        if (candidate > 2 && (candidate & 1) == 0) {
            candidate++;
            continue;
        }

        bool ok = true;
        for (size_t i = 0; i < prime_len; ++i) {
            idx_t p = prime_list[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { ok = false; break; }
        }
        if (ok) {
            prime_list[prime_len++] = candidate;
        }
        candidate++;
    }
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;          /* Undefined for index 0 */

    /* Obtain the n-th prime. */
    ensure_n_primes(n);
    seq_t p = (seq_t)prime_list[n - 1];          /* p(n)   */
    seq_t revp = (seq_t)rev((idx_t)p);           /* rev(p) */

    /* Calculate maximal m such that |p – m·revp| < p   i.e.  m·revp ≤ 2p – 1 */
    seq_t two_p_minus_one = (seq_t)2 * p - 1;
    seq_t max_m = two_p_minus_one / revp;

    seq_t count = 0;
    for (seq_t m = 1; m <= max_m; ++m) {
        seq_t prod = m * revp;
        seq_t diff = (prod > p) ? (prod - p) : (p - prod);
        if (diff == 0 || diff >= p) continue;
        if (is_prime((idx_t)diff)) ++count;
    }
    return count;
}
