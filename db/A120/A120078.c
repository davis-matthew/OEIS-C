/*
 * A120078  Coefficient triangle of numerator polynomials appearing in certain
 *          column o.g.f.s related to the H-atom spectrum.
 * https://oeis.org/A120078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 21, i32 n <= 78, i64 n <= 253,
 *            i128 n <= 1081, big any n
 * Helpers:   count_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS A120078 – Coefficient triangle of numerator polynomials
 *  Formula used in the code:
 *      a(n) is the serialised value of the triangle T(r,k) where
 *          r is the row containing index n,
 *          k = n - triangular(r-1).
 *
 *      T(r,k) = L^2 * F(k)          with
 *          L     = lcm(1,2,…,r)           (A051418(r))
 *          F(k)  = 1                     if k = 1
 *                  1/k^2 - 1/(k-1)^2      otherwise
 *               = (1-2k) / (k^2 (k-1)^2)  for k > 1
 *
 *  The implementation avoids overflow by factorising L and the
 *  denominators, multiplying only the prime powers that remain
 *  after cancellation.  This works for the 64‑bit build for all
 *  terms that fit in that type and for arbitrary‑precision
 *  builds for all n ≥ 1.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_exp.h"

/* --- helpers for prime generation and factorisation -------------------------------- */

#define MAX_N 10000          /* maximum n required by the specification */
#define PRIME_COUNT 1229     /* number of primes <= 10000 */

static uint64_t primes[PRIME_COUNT];
static size_t prime_cnt = 0;
static bool primes_ready = false;

/* Sieve of Eratosthenes – once per program */
static void build_primes(void)
{
        if (primes_ready) return;
        bool is_prime[MAX_N + 1];
        for (size_t i = 0; i <= MAX_N; ++i) is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;
        for (size_t i = 2; i * i <= MAX_N; ++i) {
                if (is_prime[i]) {
                        for (size_t j = i * i; j <= MAX_N; j += i)
                                is_prime[j] = false;
                }
        }
        for (size_t i = 2; i <= MAX_N; ++i)
                if (is_prime[i]) primes[prime_cnt++] = (uint64_t)i;
        primes_ready = true;
}

/* 10^exp computed by repeated multiplication – safe for small exponents */
static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
        seq_t res = 1;
        while (exp) {
                if (exp & 1) res = res * base;
                if (exp >>= 1) base = base * base;
        }
        return res;
}

/* floor(log_p(r)) */
static inline idx_t floor_log_p(idx_t p, idx_t r)
{
        idx_t exp = 0;
        idx_t power = p;
        while (power <= r) {
                ++exp;
                if (power > r / p) break;
                power *= p;
        }
        return exp;
}

/* ------------------------------------------------------------------------------ */

seq_t a(idx_t n)
{
        if (n == 0) return 0;  /* undefined but safe */

        /* Find the row r and column k */
        idx_t r = 1;
        idx_t sum = 1;           /* triangular number T(r) = r(r+1)/2 */
        while (sum < n) {
                ++r;
                sum += r;
        }
        idx_t prev_sum = sum - r;/* cumulative before current row */
        idx_t k = n - prev_sum;  /* 1 <= k <= r */

        build_primes();           /* ensure primes up to MAX_N are available */

        seq_t result = 1;
        for (size_t i = 0; i < prime_cnt && primes[i] <= r; ++i) {
                idx_t p = primes[i];
                idx_t e_lcm = floor_log_p(p, r);     /* exponent of p in lcm(1..r)*/
                idx_t exp   = e_lcm << 1;            /* exponent in L^2 */

                if (k > 1) {
                        idx_t e_k   = count_exp(k, p);
                        idx_t e_k1  = count_exp(k - 1, p);
                        exp -= (e_k << 1) + (e_k1 << 1);
                }
                if (exp > 0)
                        result *= pow_seq_t((seq_t)p, exp);
        }

        if (k > 1) {
                seq_t mult = (seq_t)(1) - (seq_t)(2 * k);
                result *= mult;
        }

        return result;
}
