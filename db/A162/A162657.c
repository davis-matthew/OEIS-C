/*
 * A162657  Least number m such that n is the denominator of sigma_{-1}(m), or
 *          zero if no such exists.
 * https://oeis.org/A162657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer m such that n is the denominator
 *            of σ_{‑1}(m) (i.e. σ(m)/m in lowest terms). Implemented by
 *            searching for the smallest k with m = n·k and gcd(m,σ(m)) = k
 * Range:     u8 n <= 17, u16 n <= 89, u32 n <= 2309, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code uses the helper function seq_gcd(seq_t,seq_t) and the function
   _floor_isqrt_u64(idx_t) provided in the utilities.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Utilities */
#include "../utils/seq_gcd.h"
#include "../utils/_floor_isqrt_u64.h"

/* ----------  prime table  ---------- */
#define MAX_PRIME 1300000        /* sufficient for sqrt of k up to ~1.7e12   */
static uint32_t  primes[MAX_PRIME];
static size_t    prime_cnt = 0;
static bool      primes_built = false;

/* Build a list of primes up to MAX_PRIME using a simple sieve.  */
static void build_prime_table(void)
{
    if (prime_cnt > 0)
        return;                /* already built */

    static bool is_composite[MAX_PRIME + 1] = {0};
    for (size_t i = 2; i <= MAX_PRIME; ++i) {
        if (!is_composite[i]) {
            primes[prime_cnt++] = (uint32_t)i;
            if (i <= (size_t)((double)i * (double)i / (double)i))
                for (size_t j = i * i; j <= MAX_PRIME; j += i)
                    is_composite[j] = true;
        }
    }
}

/* ----------  factorisation  ---------- */
#define MAX_FACTORS 64
static void factor_uint64(uint64_t n, uint64_t *fac, int *exp, int *cnt)
{
    int c = 0;
    uint64_t temp = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            int e = 0;
            do {
                temp /= p;
                e++;
            } while (temp % p == 0);
            fac[c]   = p;
            exp[c]   = e;
            c++;
        }
    }
    if (temp > 1) {
        fac[c]   = temp;
        exp[c]   = 1;
        c++;
    }
    *cnt = c;
}

/* ----------  main sequence function  ---------- */
seq_t a(idx_t n)
{
    /* n must be at least 1 */
    build_prime_table();

    /* factorisation of n */
    uint64_t fac_n[MAX_FACTORS];
    int     exp_n[MAX_FACTORS];
    int     cnt_n = 0;
    factor_uint64((uint64_t)n, fac_n, exp_n, &cnt_n);

    idx_t k = 1;
    for (;;) {
        idx_t   m = n * k;                    /* m = n * k */
        seq_t   sigma = 1;                    /* σ(m) */

        /* factorisation of k */
        uint64_t fac_k[MAX_FACTORS];
        int     exp_k[MAX_FACTORS];
        int     cnt_k = 0;
        factor_uint64((uint64_t)k, fac_k, exp_k, &cnt_k);

        /* merge the two factorisations to compute σ(m) */
        size_t i = 0, j = 0;
        while (i < (size_t)cnt_n || j < (size_t)cnt_k) {
            uint64_t p;
            int      exp_total;

            if (i < (size_t)cnt_n &&
               (j == (size_t)cnt_k || fac_n[i] < fac_k[j])) {
                p = fac_n[i];
                exp_total = exp_n[i];
                i++;
            } else if (j < (size_t)cnt_k &&
                      (i == (size_t)cnt_n || fac_k[j] < fac_n[i])) {
                p = fac_k[j];
                exp_total = exp_k[j];
                j++;
            } else {          /* equal primes */
                p = fac_n[i];
                exp_total = exp_n[i] + exp_k[j];
                i++; j++;
            }

            /* sum of powers: 1 + p + p^2 + ... + p^exp_total */
            seq_t term = 1;
            seq_t sum  = 1;
            for (int e = 1; e <= exp_total; ++e) {
                term = term * (seq_t)p;
                sum  = sum + term;
            }
            sigma = sigma * sum;
        }

        /* compute gcd(m, σ(m)) */
        seq_t g = seq_gcd((seq_t)m, sigma);

        /* if gcd equals k (i.e. m / gcd = n) then m is the answer */
        if (g == (seq_t)k)
            return (seq_t)m;

        /* otherwise try next k */
        k++;
    }
}
