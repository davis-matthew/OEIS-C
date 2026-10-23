/*
 * A122146  Numbers k such that q(k) = M(k) and 4*q(k) <= k, where q(k) is the
 *          largest prime divisor of k and M(k) is the largest prime power
 *          divisor of k.
 * https://oeis.org/A122146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that q(k) = M(k) and 4*q(k) ≤ k, sorted in increasing order.
   Computed by scanning integers and testing prime factorisation constraints.
*/

#include <stdbool.h>
#include <stdint.h>
#include "../utils/largest_prime_divisor.h"
#include "../utils/build_primes_upto.h"

/* static cache for primes, built once */
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static bool primes_built = false;

/* Maximum prime to precompute, sufficient for n up to ~10 000. */
#define PRIME_LIMIT 2000000

/* Build prime list once */
static void init_primes(void)
{
    if (!primes_built) {
        build_primes_upto((idx_t)PRIME_LIMIT, &primes, &prime_cnt);
        primes_built = true;
    }
}

/* Compute the largest prime power divisor of n.  n is assumed to fit in
   the provided seq_t type.  The function uses the pre‑computed prime
   list and factorises n. */
static seq_t largest_prime_power(seq_t n)
{
    seq_t temp = n;
    seq_t max_pow = 1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t pr = (seq_t)primes[i];

        /* Stop if pr^2 > temp.  Use division to avoid overflow. */
        if (pr * pr > temp) break;
        if ((temp / pr) < pr) break;          /* pr^2 > temp */

        if (temp % pr == 0) {
            seq_t cur_pow = 1;
            while (temp % pr == 0) {
                temp /= pr;
                cur_pow *= pr;
            }
            if (cur_pow > max_pow) max_pow = cur_pow;
        }
    }

    if (temp > 1) {      /* remaining prime factor */
        if (temp > max_pow) max_pow = temp;
    }
    return max_pow;
}

/* External function requested by the build. */
seq_t a(idx_t n)
{
    init_primes();

    idx_t found = 0;
    seq_t k = 2;            /* start search from the smallest possible k */

    while (true) {
        /* largest prime divisor of k must fit in uint64_t for the helper */
        uint64_t p64 = largest_prime_divisor((uint64_t)k);
        seq_t p = (seq_t)p64;

        /* 4*p <= k */
        if ((seq_t)4 * p <= k) {
            /* compute largest prime power divisor */
            seq_t M = largest_prime_power(k);
            if (M == p) {
                ++found;
                if (found == n) return k;
            }
        }
        ++k;
    }
}
