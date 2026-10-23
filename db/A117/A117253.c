/*
 * A117253  Column 0 of triangle A117252.
 * https://oeis.org/A117253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [Product_{j=0..n-1} (1-3*j)]/n! * 3^(n*(n-1)/2)
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 8, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper: fast exponentiation of seq_t for a non‑negative integer exponent */
static seq_t seq_pow_exp(seq_t base, long long exp)
{
    seq_t result = 1;
    seq_t cur = base;
    while (exp > 0) {
        if (exp & 1) result *= cur;
        exp >>= 1;
        if (exp) cur *= cur;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* sign = (-1)^(n-1) */
    seq_t sign = ((n - 1) & 1) ? (seq_t)-1 : (seq_t)1;

    /* ----- Compute primes up to 3*n ----- */
    size_t limit = 3 * (size_t)n;
    size_t prime_cap = 1000;
    unsigned int *primes = (unsigned int*)malloc(prime_cap * sizeof(unsigned int));
    size_t prime_cnt = 0;

    unsigned char *isprime = (unsigned char*)calloc(limit + 1, 1);
    if (!isprime) { free(primes); return 0; }  /* out of memory guard */

    for (size_t i = 2; i <= limit; ++i) isprime[i] = 1;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (isprime[i]) {
            for (size_t j = i * i; j <= limit; j += i) isprime[j] = 0;
        }
    }
    for (size_t i = 2; i <= limit; ++i) {
        if (isprime[i]) {
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                primes = (unsigned int*)realloc(primes, prime_cap * sizeof(unsigned int));
            }
            primes[prime_cnt++] = (unsigned int)i;
        }
    }
    free(isprime);

    /* ----- Exponent array for each prime ----- */
    long long *exps = (long long*)calloc(prime_cnt, sizeof(long long));
    if (!exps) { free(primes); return 0; } /* out of memory guard */

    /* Factor (1-3*j) for j = 1 .. n-1  (j=0 gives 1, no factors) */
    for (size_t j = 1; j < n; ++j) {
        int t = 1 - (int)(3 * j);            /* negative for j>=1 */
        int abs_t = t < 0 ? -t : t;
        int temp = abs_t;
        for (size_t pi = 0; pi < prime_cnt && (int)primes[pi] * (int)primes[pi] <= temp; ++pi) {
            unsigned int p = primes[pi];
            int cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            if (cnt) exps[pi] += cnt;
        }
        if (temp > 1) {
            /* temp is a prime > sqrt(abs_t) */
            for (size_t pi = 0; pi < prime_cnt; ++pi) {
                if (primes[pi] == (unsigned int)temp) {
                    exps[pi] += 1;
                    break;
                }
            }
        }
    }

    /* Add exponent of 3 from 3^(n*(n-1)/2) */
    long long three_exp = ((long long)n * (long long)(n - 1)) / 2;
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        if (primes[pi] == 3) {
            exps[pi] += three_exp;
            break;
        }
    }

    /* Subtract factorial exponents */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        unsigned int p = primes[pi];
        unsigned int temp_n = (unsigned int)n;
        long long e_fact = 0;
        while (temp_n) {
            temp_n /= p;
            e_fact += temp_n;
        }
        exps[pi] -= e_fact;
    }

    /* ----- Build result by multiplying primes^exp ----- */
    seq_t res = 1;
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        long long exp = exps[pi];
        if (exp <= 0) continue;
        seq_t mult = seq_pow_exp((seq_t)primes[pi], exp);
        res *= mult;
    }

    /* Apply sign and clean up */
    if (sign < 0) res = -res;
    free(primes);
    free(exps);
    return res;
}
