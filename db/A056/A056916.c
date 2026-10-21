/*
 * A056916  Product of the orders of the elements in a cyclic group with n
 *          elements.
 * https://oeis.org/A056916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Product_{d|n} d^phi(d)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute Euler's φ for a number n (0 < n < 2^64) */
static seq_t phi_uint64(idx_t n)
{
    seq_t result = (seq_t)n;          /* start with n in seq_t */
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            /* subtract factor for each prime divisor once */
            result = result / (seq_t)p * (seq_t)(p - 1);
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {                    /* remaining prime factor */
        result = result / (seq_t)tmp * (seq_t)(tmp - 1);
    }
    return result;
}

/* Helper: binary exponentiation for seq_t base and unsigned long long exp */
static seq_t pow_seq(seq_t base, unsigned long long exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1ULL) res *= base;
        base *= base;
        exp >>= 1ULL;
    }
    return res;
}

/* Helper: generate all divisors of n via recursive enumeration */
static void gen_divisors(idx_t n, idx_t factors[], int exps[], int k,
                        idx_t cur, idx_t divs[], int *cnt)
{
    if (k == 0) {
        divs[(*cnt)++] = cur;
        return;
    }
    for (int e = 0; e <= exps[k-1]; ++e) {
        gen_divisors(n, factors, exps, k-1, cur, divs, cnt);
        cur *= factors[k-1];
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Factor n into primes and exponents */
    idx_t factors[64];
    int exps[64];
    int k = 0;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            factors[k] = p;
            exps[k] = e;
            ++k;
        }
    }
    if (temp > 1) {
        factors[k] = temp;
        exps[k] = 1;
        ++k;
    }

    /* Collect all divisors */
    idx_t divs[4096];  /* ample space for <= 10000 */
    int div_count = 0;
    gen_divisors(n, factors, exps, k, 1, divs, &div_count);

    /* Compute the product over divisors */
    seq_t prod = 1;
    for (int i = 0; i < div_count; ++i) {
        idx_t d = divs[i];
        seq_t phi_d = phi_uint64(d);
        seq_t pow = pow_seq((seq_t)d, (unsigned long long)phi_d);
        prod *= pow;
    }
    return prod;
}
