/*
 * A198302  a(n) = Sum_{d|n} d * sigma(n/d, d).
 * https://oeis.org/A198302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{d|n} d * sigma(n/d, d) * where sigma(m,k) =
 *            Sum_{e|m} e^k (sum of the k‑th powers of the divisors of m)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 53, u64 n <= 113,
 *            u128 n <= 230, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* fast exponentiation of seq_t */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

/* factorization by trial division (n ≤ 10000 here) */
static size_t factorize(idx_t n, idx_t *primes, idx_t *exps)
{
    size_t cnt = 0;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            idx_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (n > 1) {
        primes[cnt] = n;
        exps[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

/* generate all divisors from prime factors */
static void gen_divisors(const idx_t *primes, const idx_t *exps, size_t cnt,
                         idx_t *divs, size_t *div_cnt)
{
    size_t cur = 1;
    divs[0] = 1;
    for (size_t i = 0; i < cnt; ++i) {
        size_t prev = cur;
        idx_t base = primes[i];
        for (idx_t k = 1; k <= exps[i]; ++k) {
            idx_t mul = 1;
            for (idx_t j = 0; j < k; ++j) mul *= base;
            for (size_t j = 0; j < prev; ++j) {
                divs[cur++] = divs[j] * mul;
            }
        }
    }
    *div_cnt = cur;
}

seq_t a(idx_t n)
{
    /* allocate storage for divisors (n ≤ 10000 → < 100 divisors) */
    const size_t MAX_DIV = 128;
    idx_t primes[64], exps[64];
    size_t prime_cnt;
    idx_t divs_n[MAX_DIV], div_cnt_n;
    size_t i;

    /* divisors of n */
    prime_cnt = factorize(n, primes, exps);
    gen_divisors(primes, exps, prime_cnt, divs_n, &div_cnt_n);

    seq_t total = (seq_t)0;

    for (i = 0; i < div_cnt_n; ++i) {
        idx_t d = divs_n[i];
        idx_t m = n / d;  /* n/d */

        /* divisors of m */
        idx_t primes_m[64], exps_m[64];
        size_t prime_cnt_m;
        idx_t divs_m[MAX_DIV], div_cnt_m;

        prime_cnt_m = factorize(m, primes_m, exps_m);
        gen_divisors(primes_m, exps_m, prime_cnt_m, divs_m, &div_cnt_m);

        for (size_t j = 0; j < div_cnt_m; ++j) {
            idx_t e = divs_m[j];
            seq_t e_pow = pow_seq((seq_t)e, d);
            seq_t term = (seq_t)d * e_pow;
            total += term;
        }
    }

    return total;
}
