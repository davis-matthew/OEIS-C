/*
 * A060506  Numerators of the asymptotic expansion of the Airy function Ai(x).
 * https://oeis.org/A060506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = numerator( ∏_{k=1}^{3n‑1} (2k+1) / (216^n·n!) )
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by prime‑factorizing the numerator and
   denominator, subtracting exponents, and multiplying the remaining
   primes.  A simple sieve up to 6n‑1 is used to obtain all
   primes needed for factorisation.  All arithmetic is performed with
   the seq_t type; the code is valid C11 and C++17. */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MAXLIMIT 60000            /* 6·10000‑1 ≈ 60000, enough for n≤10000 */

/* static tables */
static int  primes[MAXLIMIT + 1];
static int  primeCount = 0;
static int  currentSieveLimit = 0;

static int  expNum[MAXLIMIT + 1];   /* exponents of primes in numerator   */
static int  expDen[MAXLIMIT + 1];   /* exponents of primes in denominator */

/* fast exponentiation for seq_t */
static seq_t seq_pow(seq_t base, int exp)
{
    seq_t result = 1;
    seq_t pow    = base;
    while (exp > 0) {
        if (exp & 1) result *= pow;
        pow *= pow;
        exp >>= 1;
    }
    return result;
}

/* compute primes up to 'limit' using a simple sieve */
static void compute_primes(unsigned int limit)
{
    if (limit <= currentSieveLimit) return;     /* already enough */

    char isPrime[MAXLIMIT + 1];
    memset(isPrime, 1, sizeof(isPrime));
    isPrime[0] = isPrime[1] = 0;

    unsigned int i;
    for (i = 2; i * i <= limit; ++i)
        if (isPrime[i])
            for (unsigned int j = i * i; j <= limit; j += i)
                isPrime[j] = 0;

    primeCount = 0;
    for (i = 2; i <= limit; ++i)
        if (isPrime[i])
            primes[primeCount++] = (int)i;

    currentSieveLimit = limit;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* empty product */

    /* maximum odd number in the product is 6n-1 */
    unsigned int limit = 6 * (unsigned int)n - 1;
    compute_primes(limit);

    /* reset exponent tables up to 'limit' */
    memset(expNum, 0, (limit + 1) * sizeof(int));
    memset(expDen, 0, (limit + 1) * sizeof(int));

    /* factorise numerator ∏_{k=1}^{3n-1} (2k+1) */
    for (idx_t k = 1; k <= 3 * n - 1; ++k) {
        int value = 2 * (int)k + 1;          /* 3,5,7,...,6n-1 */
        int t = value;
        for (int pi = 0; pi < primeCount; ++pi) {
            int p = primes[pi];
            if (p * p > t) break;
            while (t % p == 0) {
                ++expNum[p];
                t /= p;
            }
        }
        if (t > 1) ++expNum[t];              /* remaining prime */
    }

    /* factorise denominator: 216^n * n! */
    expDen[2] += 3 * (int)n;  /* 216 = 2^3 * 3^3 */
    expDen[3] += 3 * (int)n;

    for (int i = 2; i <= (int)n; ++i) {
        int t = i;
        for (int pi = 0; pi < primeCount; ++pi) {
            int p = primes[pi];
            if (p * p > t) break;
            while (t % p == 0) {
                ++expDen[p];
                t /= p;
            }
        }
        if (t > 1) ++expDen[t];
    }

    /* assemble the result: product of primes raised to
       difference of exponents (numerator > denominator) */
    seq_t result = 1;
    for (int pi = 0; pi < primeCount; ++pi) {
        int p = primes[pi];
        int diff = expNum[p] - expDen[p];
        if (diff > 0)
            result *= seq_pow((seq_t)p, diff);
    }

    return result;
}
