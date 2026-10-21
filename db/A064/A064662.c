/*
 * A064662  a(n) = n - round( prime(n)/log(prime(n)) ).
 * https://oeis.org/A064662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n - round( prime(n)/log(prime(n)) )
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* maximum prime needed for n up to about 10000: 10000th prime is 104729 */
#define MAX_PRIME 200000
#define MAX_PRIME_COUNT 20000

/* build the primes array once */
static void build_primes(idx_t *primes, size_t *cnt)
{
    static uint8_t sieve[MAX_PRIME + 1];
    size_t limit = MAX_PRIME;

    memset(sieve, 1, limit + 1);
    sieve[0] = sieve[1] = 0;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                sieve[j] = 0;
        }
    }

    *cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (sieve[i]) {
            primes[(*cnt)++] = (idx_t)i;
            if (*cnt >= MAX_PRIME_COUNT)
                break;
        }
    }
}

/* the requested sequence */

seq_t a(idx_t n)
{
    static int initialized = 0;
    static idx_t primes[MAX_PRIME_COUNT];
    static size_t primes_cnt = 0;

    if (!initialized) {
        build_primes(primes, &primes_cnt);
        initialized = 1;
    }

    if (n == 0)          /* by definition the sequence starts at n = 1 */
        return (seq_t)0;

    /* safety: ensure we have enough primes */
    if ((unsigned long)n > primes_cnt) {
        /* This shouldn't happen for the intended limits (n ≤ 10000) */
        /* Fallback: return 0 */
        return (seq_t)0;
    }

    idx_t p = primes[n - 1];                     /* p_n */
    double pd = (double)p;
    double val = pd / log(pd);                   /* p_n / ln(p_n) */
    long long r = llround(val);                  /* round to nearest integer */

    seq_t result = (seq_t)( (long long)n - (long long)r );
    return result;
}
