/*
 * A199918  Expansion of false theta series variation of Euler's pentagonal
 *          number series in powers of x.
 * https://oeis.org/A199918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = b(24*n + 1) Multiplicative b has rules: • b(p^k) = 0 if k
 *            is odd or p == 2 or p == 3 • if p % 24 == 1,5,7,11 then
 *            b(p^(2e)) = +1 • if p % 24 == 13,17,19,23 then b(p^(2e)) =
 *            (-1)^e a(0) = 1, a(n) takes values 1, 0 or -1
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define LIMIT 65536          /* enough for prime table up to sqrt(24*n+1) for n≈10⁶ */

static uint32_t primes[LIMIT];
static size_t prime_cnt = 0;

/* Generate primes up to LIMIT once. */
static void init_primes(void)
{
    if (prime_cnt != 0) return;          /* already done */

    bool sieve[LIMIT] = {0};

    for (size_t i = 2; i < LIMIT; ++i)
        if (!sieve[i]) {
            primes[prime_cnt++] = (uint32_t)i;
            if (i * i < LIMIT)
                for (size_t j = i * i; j < LIMIT; j += i)
                    sieve[j] = true;
        }
}

/* Compute term a(n) of the sequence. */
seq_t a(idx_t n)
{
    init_primes();

    /* Compute m = 24*n + 1. */
    uint64_t m = 24ULL * n + 1ULL;
    if (m == 1ULL) return (seq_t)1;

    /* If m is divisible by 2 or 3 the multiplicative function is zero. */
    if ((m & 1ULL) == 0ULL || (m % 3ULL) == 0ULL)
        return (seq_t)0;

    uint64_t tmp = m;
    int sign = 1;

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        int exp = 0;
        while ((tmp % p) == 0) {
            ++exp;
            tmp /= p;
        }
        if (exp == 0) continue;

        /* odd exponent → 0. */
        if (exp & 1) return (seq_t)0;

        uint32_t rem = p % 24;
        if (rem == 13 || rem == 17 || rem == 19 || rem == 23) {
            int e = exp / 2;
            if (e & 1) sign = -sign;           /* flip sign if e is odd */
        }
        /* rem == 1,5,7,11 contributes +1; nothing to do. */
    }

    /* Remaining factor > sqrt(m) gives exponent 1 (odd). */
    if (tmp > 1ULL) return (seq_t)0;

    return (seq_t)sign;
}
