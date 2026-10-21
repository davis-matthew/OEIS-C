/*
 * A058270  An approximation to sigma_{3/2}(n): round( sum_{d|n} d^(3/2) ).
 * https://oeis.org/A058270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round( sum_{d|n} d^(3/2) )
 * Range:     u8 n <= 29, u16 n <= 1079, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Integer square root for unsigned __int128 (floor), based on bitwise algorithm */
static unsigned __int128 isqrt128(unsigned __int128 n)
{
    unsigned __int128 x = 0;
    unsigned __int128 bit = ((unsigned __int128)1 << 126);  /* highest power of four <= n */
    while (bit > n) bit >>= 2;
    while (bit) {
        unsigned __int128 tmp = x + bit;
        if (tmp <= n) {
            n -= tmp;
            x = (x >> 1) + bit;
        } else {
            x >>= 1;
        }
        bit >>= 2;
    }
    return x;
}

/* Simple sieve to generate primes up to 10000 */
static void init_primes(int *primes, size_t *cnt)
{
    const int MAX = 10000;
    bool sieve[MAX + 1];
    for (int i = 0; i <= MAX; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (int i = 2; i * i <= MAX; ++i) {
        if (sieve[i]) {
            for (int j = i * i; j <= MAX; j += i) sieve[j] = false;
        }
    }
    *cnt = 0;
    for (int i = 2; i <= MAX; ++i) if (sieve[i]) primes[(*cnt)++] = i;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* static data */
    static bool initialized = false;
    static int primes[1229];          /* number of primes <= 10000 */
    static size_t prime_count = 0;

    if (!initialized) {
        init_primes(primes, &prime_count);
        initialized = true;
    }

    /* factor n */
    idx_t tmp = n;
    struct pair {idx_t p; int e;} factors[16];
    size_t fcnt = 0;
    for (size_t i = 0; i < prime_count && (idx_t)primes[i] * (idx_t)primes[i] <= tmp; ++i) {
        if (tmp % primes[i] == 0) {
            int e = 0;
            while (tmp % primes[i] == 0) {
                tmp /= primes[i];
                ++e;
            }
            factors[fcnt].p = primes[i];
            factors[fcnt].e = e;
            ++fcnt;
        }
    }
    if (tmp > 1) {
        factors[fcnt].p = tmp;
        factors[fcnt].e = 1;
        ++fcnt;
    }

    /* generate all divisors via recursion */
    idx_t divs[256];
    size_t dcnt = 0;
    divs[dcnt++] = 1;
    for (size_t i = 0; i < fcnt; ++i) {
        idx_t p = factors[i].p;
        int e = factors[i].e;
        size_t cur = dcnt;
        for (int exp = 1; exp <= e; ++exp) {
            idx_t mult = (idx_t)1;
            for (int e2 = 0; e2 < exp; ++e2) mult *= p;
            for (size_t j = 0; j < cur; ++j) {
                divs[dcnt++] = divs[j] * mult;
            }
        }
    }

    /* compute sum of d*sqrt(d) with 48 fractional bits */
    const int K = 48;
    const unsigned __int128 HALF = ((unsigned __int128)1 << K);
    unsigned __int128 sum_scaled = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        idx_t d = divs[i];
        unsigned __int128 val = ((unsigned __int128)d) << (2 * K);
        unsigned __int128 s   = isqrt128(val);          /* floor(sqrt(d) * 2^K) */
        unsigned __int128 term_scaled = ((unsigned __int128)d) * s;
        sum_scaled += term_scaled;
    }

    /* rounding to nearest integer: (sum_scaled + 2^(K-1)) >> K */
    seq_t result = (seq_t)((sum_scaled + (HALF >> 1)) >> K);
    return result;
}
