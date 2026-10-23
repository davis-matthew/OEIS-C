/*
 * A030188  Expansion of q^(-1/2) * eta(q) * eta(q^2) * eta(q^3) * eta(q^6) in
 *          powers of q.
 * https://oeis.org/A030188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(2*n + 1), where b is multiplicative with b(1)=1,
 *            b(p^e)=b(p)*b(p^(e-1)) - p*b(p^(e-2)) for primes p≠2,3; special
 *            case: b(2^e)=0^e, b(3^e)=(-1)^e. For prime powers, b(p)=p+1 -
 *            #E(F_p) where E: y^2 = x^3 - x^2 - 4x + 4 and #E(F_p) counts
 *            points over F_p including infinity
 * Range:     i8 n <= 1160, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* eslint-disable-next-line inlinemarkdown -- just to keep the comment readable */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Maximum n expected plus one so that m = 2*n+1 <= MAXN */
#define MAXN 40001   /* covers n up to 20000 comfortably */

/* Helper structure for prime data */
typedef struct {
    int prime;      /* prime value */
    int maxExp;     /* maximum exponent such that prime^exp <= MAXN */
    seq_t *bvals;   /* b(prime^e) values for e=0..maxExp */
} PrimeInfo;

/* Global data initialized lazily */
static bool initialized = false;
static int primeCnt = 0;
static int *primeList = NULL;          /* list of primes <= MAXN */
static int *primeIdxMap = NULL;        /* map prime -> index in primeInfo array (-1 if not prime) */
static PrimeInfo *primeInfos = NULL;   /* array of prime data */

/* Compute modulo power (uint64_t) for Legendre symbol when needed */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}

/* Compute trace (a_p) for an odd prime p > 2: a_p = p - Npoints,
   where Npoints counts affine points of E over F_p. */
static int compute_trace_prime(int p)
{
    /* temporary buffer for squares */
    static unsigned char buf[MAXN + 1] = {0};
    int P = p;
    int i;

    /* clear buffer for this prime */
    for (i = 0; i < P; ++i) buf[i] = 0;

    /* fill with squares */
    for (i = 0; i < P; ++i) {
        int sq = (int)((1LL * i * i) % P);
        buf[sq] = 1;
    }

    int Npoints = 0;
    for (i = 0; i < P; ++i) {
        int64_t xx = (int64_t)i * i % P;        /* x^2 mod p */
        int64_t xxx = (int64_t)xx * i % P;      /* x^3 mod p */
        int64_t fourx = (4LL * i) % P;          /* 4*x mod p */
        int rhs = (int)((xxx - xx - fourx + 4) % P);
        if (rhs < 0) rhs += P;
        if (rhs == 0)
            Npoints += 1;                 /* y = 0 only */
        else if (buf[rhs])
            Npoints += 2;                 /* two solutions for y */
    }

    /* include the point at infinity in total points,
       thus total #E(F_p) = Npoints + 1 */
    return p - Npoints;   /* p + 1 - (Npoints + 1) = p - Npoints */
}

/* Initialize global data (sieve, traces, b values) */
static void init_sequence(void)
{
    /* sieve for primes up to MAXN */
    bool *sieve = (bool *)malloc((MAXN + 1) * sizeof(bool));
    if (!sieve) exit(EXIT_FAILURE);
    for (int i = 0; i <= MAXN; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (int i = 2; (int64_t)i * i <= MAXN; ++i) {
        if (sieve[i]) {
            for (int j = i * i; j <= MAXN; j += i) sieve[j] = false;
        }
    }

    /* Count primes and allocate prime list */
    primeCnt = 0;
    for (int i = 2; i <= MAXN; ++i) if (sieve[i]) primeCnt++;
    primeList = (int *)malloc(primeCnt * sizeof(int));
    primeIdxMap = (int *)malloc((MAXN + 1) * sizeof(int));
    for (int i = 0; i <= MAXN; ++i) primeIdxMap[i] = -1;

    int idx = 0;
    for (int i = 2; i <= MAXN; ++i) if (sieve[i]) {
        primeList[idx] = i;
        primeIdxMap[i] = idx;
        idx++;
    }

    /* Allocate per-prime data */
    primeInfos = (PrimeInfo *)malloc(primeCnt * sizeof(PrimeInfo));
    for (int pi = 0; pi < primeCnt; ++pi) {
        int p = primeList[pi];
        int exp = 0;
        int64_t pow = 1;
        while (pow * (int64_t)p <= MAXN) {
            pow *= p;
            exp++;
        }
        primeInfos[pi].prime = p;
        primeInfos[pi].maxExp = exp;
        primeInfos[pi].bvals = (seq_t *)malloc((exp + 1) * sizeof(seq_t));

        /* b(1) */
        primeInfos[pi].bvals[0] = (seq_t)1;

        /* handle special prime 3 */
        if (p == 3) {
            primeInfos[pi].bvals[1] = (seq_t)-1;
            for (int e = 2; e <= exp; ++e) {
                seq_t prev = primeInfos[pi].bvals[e - 1];
                seq_t prevprev = primeInfos[pi].bvals[e - 2];
                primeInfos[pi].bvals[e] = prev * primeInfos[pi].bvals[1] - (seq_t)p * prevprev;
            }
        } else {
            /* ordinary prime: compute trace (b(p)) */
            int trace = compute_trace_prime(p);
            primeInfos[pi].bvals[1] = (seq_t)trace;
            for (int e = 2; e <= exp; ++e) {
                seq_t prev = primeInfos[pi].bvals[e - 1];
                seq_t prevprev = primeInfos[pi].bvals[e - 2];
                primeInfos[pi].bvals[e] = prev * primeInfos[pi].bvals[1] - (seq_t)p * prevprev;
            }
        }
    }

    /* Free temporary sieve */
    free(sieve);
    initialized = true;
}

/* Main function to compute a(n) */
seq_t a(idx_t n)
{
    if (!initialized) init_sequence();

    /* compute m = 2*n + 1 */
    idx_t m = n * 2ULL + 1ULL;
    if (m == 1ULL) return (seq_t)1;   /* a(0) = 1 */

    seq_t result = (seq_t)1;
    idx_t rem = m;
    /* factorization by trial division */
    for (int i = 0; i < primeCnt; ++i) {
        int p = primeList[i];
        idx_t p64 = (idx_t)p;
        if (p64 * p64 > rem) break;
        if (rem % p64 == 0) {
            int exp = 0;
            while (rem % p64 == 0) {
                rem /= p64;
                exp++;
            }
            seq_t bval;
            if (p == 3) {
                /* b(3^exp) = (-1)^exp */
                bval = (exp % 2 == 0) ? (seq_t)1 : (seq_t)-1;
            } else {
                bval = primeInfos[primeIdxMap[p]].bvals[exp];
            }
            result *= bval;
        }
    }
    /* any remaining prime factor > sqrt(rem) */
    if (rem > 1) {
        int p = (int)rem;   /* rem fits in int because rem <= MAXN */
        seq_t bval;
        if (p == 3) {
            bval = -1;
        } else {
            bval = primeInfos[primeIdxMap[p]].bvals[1];
        }
        result *= bval;
    }
    return result;
}
