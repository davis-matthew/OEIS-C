/*
 * A065681  Number of primes <= prime(n) which begin with a 2.
 * https://oeis.org/A065681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of primes <= prime(n) whose decimal representation
 *            starts with digit 2
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 103 terms through n = 989 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>

/* check if p starts with digit 2 */
static inline bool
starts_with_2(uint64_t p)
{
    while (p >= 10ULL) p /= 10ULL;
    return p == 2ULL;
}

/* estimate an upper bound for the nth prime */
static unsigned long long
upper_limit_for_n(idx_t n)
{
    if (n < 6) return 15ULL;
    double x = (double)n;
    double lim = x * (log(x) + log(log(x)));
    return (unsigned long long)(lim) + 3ULL;   /* safety margin */
}

/* main function */
seq_t a(idx_t n)
{
    unsigned long long limit = upper_limit_for_n(n);
    size_t size = limit + 1U;
    unsigned char *sieve = (unsigned char *)malloc(size);
    if (!sieve) return (seq_t)0;  /* fallback on allocation failure */

    /* simple sieve */
    for (size_t i = 0; i < size; ++i) sieve[i] = 1U;
    sieve[0] = sieve[1] = 0U;
    for (unsigned long long i = 2ULL; i * i <= limit; ++i)
        if (sieve[i])
            for (unsigned long long j = i * i; j <= limit; j += i)
                sieve[j] = 0U;

    /* scan primes */
    idx_t prime_cnt = 0;
    seq_t count_2 = 0;
    for (unsigned long long p = 2ULL; p <= limit; ++p)
    {
        if (!sieve[p]) continue;
        ++prime_cnt;
        if (starts_with_2(p)) ++count_2;
        if (prime_cnt == n) break;
    }

    free(sieve);
    return count_2;
}
