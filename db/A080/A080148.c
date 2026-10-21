/*
 * A080148  Positions of primes of the form 4*k+3 (A002145) among all primes
 *          (A000040).
 * https://oeis.org/A080148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the index (among all primes) of the n‑th prime that
 *            satisfies p ≡ 3 (mod 4). (OEIS A080148)
 * Range:     u8 n <= 132, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_idx_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/grow_idx_array.h"

/* static state ---------------------------------------------------------- */

/* List of all primes found so far.  primes[i] is the (i+1)-th prime.   */
static idx_t *primes = NULL;
static size_t primes_num = 0, primes_cap = 0;

/* For each prime ≡ 3 (mod 4), store its 1‑based index among all primes. */
static idx_t *spec_idx = NULL;
static size_t spec_num = 0, spec_cap = 0;

/* maximum integer that has been sieved so far (inclusive).                  */
static idx_t limit_sieved = 0;

/* helper functions ------------------------------------------------------- */

/* trial‑division primality test using the list of primes seen so far.     */
static bool is_prime_trial(idx_t x)
{
    if (x < 2) return false;
    if (x == 2) return true;
    if ((x & 1) == 0) return false;
    idx_t r = (idx_t) sqrt((double)x);
    for (size_t i = 0; i < primes_num; ++i) {
        idx_t p = primes[i];
        if (p > r) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* extend the prime list up to a given numerical limit (inclusive).       */
static void extend_to(idx_t new_limit)
{
    if (new_limit <= limit_sieved) return;

    for (idx_t x = limit_sieved + 1; x <= new_limit; ++x) {
        if (is_prime_trial(x)) {
            /* add prime *after* checking divisibility */
            if (primes_num == primes_cap) {
                grow_idx_array(&primes, &primes_num, &primes_cap, 16);
            }
            primes[primes_num++] = x;

            /* if prime ≡ 3 (mod 4), record its index (1‑based) */
            if ((x & 3) == 3) {
                if (spec_num == spec_cap) {
                    grow_idx_array(&spec_idx, &spec_num, &spec_cap, 16);
                }
                spec_idx[spec_num++] = (idx_t)primes_num;  /* primes_num is 1‑based */
            }
        }
    }
    limit_sieved = new_limit;
}

/* estimate an upper bound for the n‑th 3‑mod‑4 prime index.                */
/* The proportion of such primes is about ½, so we need roughly 2n
   ordinary primes.  For the k‑th prime we use Dusart's bound:
      p_k <= k (ln k + ln ln k)  for k >= 6. */
static idx_t estimate_upper(int n)
{
    if (n < 1) return 0;
    idx_t k = 2 * (idx_t)n;
    if (k < 6) return 20;   /* small constant enough for tiny n            */
    double kd = (double)k;
    double est = kd * (log(kd) + log(log(kd)));
    return (idx_t)(est + 10.0);   /* safety margin                           */
}

/* public function -------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe              */

    /* Ensure we have at least n 3‑mod‑4 primes recorded.                */
    while (spec_num < (size_t)n) {
        idx_t limit = estimate_upper((int)n);
        if (limit <= limit_sieved) limit = limit_sieved + 1;
        extend_to(limit);
        if (spec_num >= (size_t)n) break;
        /* if still insufficient, double the limit */
        extend_to(limit * 2);
    }

    /* indices are stored 1‑based; n is 1‑based.                         */
    return (seq_t) spec_idx[n - 1];
}
