/*
 * A203467  a(n) = A203309(n)/A000178(n) where A000178 are superfactorials.
 * https://oeis.org/A203467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{k=1..n-1} (2*k+2)!/(2^k*(k+2)!)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper for integer exponentiation of seq_t by small exponent */
static seq_t pow_seq(seq_t base, int exp)
{
    seq_t result = (seq_t)1;
    seq_t b = base;
    int e = exp;
    while (e) {
        if (e & 1)
            result *= b;
        e >>= 1;
        if (e)
            b *= b;
    }
    return result;
}

/* Legendre exponent of prime p in m! */
static int legendre(idx_t m, uint32_t p)
{
    int e = 0;
    while (m) {
        m /= p;
        e += (int)m;
    }
    return e;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    /* 1. Generate primes up to 2*n+2 using simple sieve */
    idx_t limit = 2 * n + 2;
    /* Allocate boolean array for composite flags */
    char *is_comp = (char*)malloc(limit + 1);
    if (!is_comp)
        return (seq_t)0;          /* allocation failure */
    for (idx_t i = 0; i <= limit; ++i)
        is_comp[i] = 0;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (!is_comp[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;

    /* Count primes */
    size_t prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[i])
            ++prime_cnt;

    /* Store primes in array */
    uint32_t *primes = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    if (!primes) {
        free(is_comp);
        return (seq_t)0;
    }
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[i])
            primes[idx++] = (uint32_t)i;

    free(is_comp);

    /* 2. Accumulate exponents for each prime */
    int *exp = (int*)calloc(prime_cnt, sizeof(int));
    if (!exp) {
        free(primes);
        return (seq_t)0;
    }

    for (idx_t k = 1; k <= n - 1; ++k) {
        idx_t a = 2 * k + 2;
        idx_t b = k + 2;
        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            int e = legendre(a, primes[pi]) - legendre(b, primes[pi]);
            exp[pi] += e;
        }
    }

    /* Subtract total power of 2 from denominators: sum_{k=1}^{n-1} k = n*(n-1)/2 */
    /* Find index of prime 2 */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        if (primes[pi] == 2) {
            idx_t sumk = (n - 1) * n / 2;
            exp[pi] -= (int)sumk;
            break;
        }
    }

    /* 3. Compute final result from prime exponents */
    seq_t ans = (seq_t)1;
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        int e = exp[pi];
        if (e > 0)
            ans *= pow_seq((seq_t)primes[pi], e);
    }

    free(primes);
    free(exp);
    return ans;
}
