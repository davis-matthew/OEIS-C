/*
 * A058256  a(n) = A058254(n+1)/A058254(n).
 * https://oeis.org/A058256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm_{i=1..n+1} (prime(i)-1) / lcm_{i=1..n} (prime(i)-1)
 * Range:     u8 n <= 101, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_small_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/mul_small_pow.h"

/*--------------------------------------------------------------------
   Precomputed data
--------------------------------------------------------------------*/
#define MAXP 200000      /* upper bound for sieve (covers >10 000‑th prime)   */
#define MAXN 200000      /* maximum n handled safely by this implementation */

static int  primes[MAXP+1];      /* list of primes (0‑based, primes[0]=2)         */
static int  primeIdxMap[MAXP+1]; /* prime value → index in primes[] array, -1 = composite */
static int  primeExp[MAXP+1];    /* current maximal exponent for each prime          */
static idx_t computedUpTo = 0;   /* largest n for which a(n) has been computed   */
static seq_t aVals[MAXN+1];      /* a(1) … a(MAXN); aVals[0] unused               */
static bool  initialized   = false; /* whether tables have been built */

static void init(void)
{
    /* sieve primes up to MAXP */
    static unsigned char isPrime[MAXP+1];
    for (int i = 2; i <= MAXP; ++i) isPrime[i] = 1;
    for (int i = 2; i * i <= MAXP; ++i)
        if (isPrime[i])
            for (int j = i * i; j <= MAXP; j += i)
                isPrime[j] = 0;

    /* build primes array and mapping index */
    int cnt = 0;
    for (int i = 2; i <= MAXP; ++i) {
        if (isPrime[i]) {
            primes[cnt] = i;
            primeIdxMap[i] = cnt;
            ++cnt;
        }
    }
    /* mark composites */
    for (int i = 2; i <= MAXP; ++i)
        if (primeIdxMap[i] == 0 && i != 2)
            primeIdxMap[i] = -1;

    /* initialise prime exponents to 0 */
    for (int i = 0; i < cnt; ++i)
        primeExp[i] = 0;
    /* aVals[0] unused; aVals[1] will be set on first call */
    aVals[0] = 0;
    computedUpTo = 0;
    initialized = true;
}

/*--------------------------------------------------------------------
   Main sequence function
--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (!initialized)
        init();

    /* return cached value if available */
    if (n <= computedUpTo)
        return aVals[n];

    /* extend computation up to n */
    for (idx_t k = computedUpTo + 1; k <= n; ++k) {
        int primeVal = primes[(int)k];          /* prime(k+1)  (primes[0] = 2)   */
        unsigned int w = (unsigned int)primeVal - 1;
        unsigned int tmp = w;
        unsigned __int128 ratio = 1;

        /* factorise w using all primes up to sqrt(w) */
        for (int pi = 0; pi < MAXP && (unsigned int)primes[pi] * primes[pi] <= tmp; ++pi) {
            int p = primes[pi];
            if (tmp % p == 0) {
                int exp = 0;
                while (tmp % p == 0) {
                    tmp /= p;
                    ++exp;
                }
                int idx = primeIdxMap[p];
                if (exp > primeExp[idx]) {
                    int diff = exp - primeExp[idx];
                    primeExp[idx] = exp;
                    ratio = mul_small_pow((unsigned int)p, diff, ratio);
                }
            }
        }

        /* remaining large prime factor, if any */
        if (tmp > 1) {                    /* tmp itself is prime */
            int p = tmp;
            int idx = primeIdxMap[p];
            if (1 > primeExp[idx]) {
                primeExp[idx] = 1;
                ratio *= p;
            }
        }

        aVals[(int)k] = (seq_t)ratio;      /* cast works for all seq_t types */
        computedUpTo = k;
    }

    return aVals[(int)n];
}
