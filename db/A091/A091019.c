/*
 * A091019  Denominators of the coefficients in the expansion of
 *          arccosh(x)/sqrt(2*(x-1)) about x=1.
 * https://oeis.org/A091019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = denominator of the reduced fraction (2n)! / ((2n+1)(n!)^2
 *            8^n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   exp_in_single
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A091019: Denominators of the coefficients in   
   arccosh(x)/sqrt(2*(x-1)) about x=1                     */

#include <stdint.h>
#include "../utils/exp_in_single.h"

static uint32_t primes[3000];
static int prime_count = 0;
static idx_t max_prime_checked = 1;

static void ensure_primes(idx_t limit)
{
    if (limit <= max_prime_checked)
        return;
    for (idx_t cand = max_prime_checked + 1; cand <= limit; ++cand) {
        int is_prime = 1;
        for (int i = 0; i < prime_count && (uint64_t)primes[i] * primes[i] <= cand; ++i) {
            if (cand % primes[i] == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime)
            primes[prime_count++] = (uint32_t)cand;
    }
    max_prime_checked = limit;
}

static idx_t exp_factorial(idx_t n, uint32_t p)
{
    idx_t sum = 0;
    while (n) {
        n /= p;
        sum += n;
    }
    return sum;
}

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    idx_t limit = 2 * n + 1;
    ensure_primes(limit);

    seq_t result = (seq_t)1;
    for (int i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        if (p > limit)
            break;

        idx_t exp_numer = exp_factorial(2 * n, p);
        idx_t exp_denom_init = 2 * exp_factorial(n, p);
        if (p == 2)
            exp_denom_init += 3 * n;        /* 8^n contributes 3n powers of 2 */
        exp_denom_init += exp_in_single(2 * n + 1, p); /* factor (2n+1) */

        idx_t exp_denom;
        if (exp_numer < exp_denom_init)
            exp_denom = exp_denom_init - exp_numer;
        else
            exp_denom = 0;

        if (exp_denom) {
            seq_t term = pow_seq((seq_t)p, exp_denom);
            result *= term;
        }
    }
    return result;
}
