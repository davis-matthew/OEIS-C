/*
 * A034737  Dirichlet convolution of b_n=2^(n-1) with sigma(n).
 * https://oeis.org/A034737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n} 2^(d-1) * sigma(n/d) (Dirichlet convolution of
 *            2^(n-1) with sigma(n))
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: generate all divisors of n from its prime factorization.
   `pr` and `ex` contain the primes and their exponents (cnt elements). */
static void gen_div(size_t idx, size_t total, idx_t cur,
                    const uint64_t *pr, const uint64_t *ex,
                    idx_t *out, size_t *outcnt)
{
    if (idx == total) {
        out[(*outcnt)++] = cur;
        return;
    }
    uint64_t p = pr[idx];
    idx_t pow = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        gen_div(idx + 1, total, cur * pow, pr, ex, out, outcnt);
        pow *= (idx_t)p;
    }
}

/* Helper: compute sigma(m) = sum of divisors of m.
   Factorization is performed by trial division (m <= n <= 10000). */
static seq_t sigma_of_uint64(uint64_t m)
{
    if (m == 0)
        return 0;

    seq_t res = 1;
    uint64_t t = m;

    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            uint64_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            // Compute p^(e+1)
            seq_t power = 1;
            for (uint64_t k = 0; k < e + 1; ++k)
                power *= (seq_t)p;
            seq_t sigma_factor = (power - 1) / (seq_t)(p - 1);
            res *= sigma_factor;
        }
    }
    if (t > 1) { /* remaining prime factor with exponent 1 */
        seq_t power = (seq_t)t * (seq_t)t; /* t^(1+1) */
        seq_t sigma_factor = (power - 1) / (seq_t)(t - 1);
        res *= sigma_factor;
    }
    return res;
}

/* Main function for OEIS sequence A034737 */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n<1, but return 0 for safety */

    /* Factor n into primes and exponents */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    uint64_t temp = (uint64_t)n;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (temp > 1) {
        primes[cnt] = temp;
        exps[cnt] = 1;
        ++cnt;
    }

    /* Generate all divisors of n */
    idx_t divisors[1024];
    size_t dcnt = 0;
    gen_div(0, cnt, 1, primes, exps, divisors, &dcnt);

    /* Compute the convolution sum */
    seq_t sum = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        idx_t d = divisors[i];
        /* Compute 2^(d-1) */
        seq_t two = 1;
        for (idx_t k = 0; k < d - 1; ++k)
            two *= 2;

        idx_t nd = n / d;
        seq_t sigma = sigma_of_uint64((uint64_t)nd);

        sum += two * sigma;
    }

    return sum;
}
