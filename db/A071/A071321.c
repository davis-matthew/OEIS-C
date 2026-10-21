/*
 * A071321  Alternating sum of all prime factors of n; primes nondecreasing,
 *          starting with the least prime factor: A020639(n).
 * https://oeis.org/A071321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{i=1}^{k} (-1)^{i-1} p_i where p_i are the prime
 *            factors of n (with multiplicity), listed in nondecreasing order
 * Range:     i8 n <= 130, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Pre‑compute primes up to 1 000 000 once.  That covers any n≤10^12,    */
/* which is far beyond the range of the tests (~10 000).  The array size  */
/* 78498 primes is safe for 32‑bit indices.                              */
/* --------------------------------------------------------------------- */
#define MAX_PRIME_LIMIT 1000000
#define MAX_PRIME_CNT   78500

static uint32_t _primes[MAX_PRIME_CNT];
static size_t   _prime_cnt = 0;
static bool     _primes_ready = false;

static void _init_primes(void)
{
    if (_primes_ready) return;
    static unsigned char sieve[MAX_PRIME_LIMIT + 1];
    for (unsigned int i = 0; i <= MAX_PRIME_LIMIT; ++i) sieve[i] = 0;
    sieve[0] = sieve[1] = 1;
    for (unsigned int i = 2; (unsigned long long)i * i <= MAX_PRIME_LIMIT; ++i)
        if (!sieve[i])
            for (unsigned int j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                sieve[j] = 1;
    for (unsigned int i = 2; i <= MAX_PRIME_LIMIT; ++i)
        if (!sieve[i])
            _primes[_prime_cnt++] = i;
    _primes_ready = true;
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;               /* a(1)=0 */

    _init_primes();

    uint64_t tmp = (uint64_t)n;          /* work with a copy */
    int      sign = 1;                   /* +1 for first factor */
    __int128 sum = 0;                    /* accumulate in 128‑bit */

    for (size_t i = 0; i < _prime_cnt; ++i) {
        uint32_t p = _primes[i];
        if ((uint64_t)p * p > tmp) break;
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            for (int j = 0; j < exp; ++j) {
                sum += (int64_t)sign * (int64_t)p;
                sign = -sign;
            }
        }
    }
    /* any remaining factor > sqrt(original) is prime */
    if (tmp > 1) {
        sum += (int64_t)sign * (int64_t)tmp;
        /* sign = -sign;  /* not needed after last term */
    }

    return (seq_t)sum; /* cast to the required integer type */
}
