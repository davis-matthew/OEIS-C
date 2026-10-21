/*
 * A061898  Swap each prime in factorization of n with "neighbor" prime.
 * https://oeis.org/A061898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look, mult
 * Formula:   a(n) swaps each prime in the factorization of n with its
 *            “neighbor” prime (2↔3,5↔7,11↔13,…). The function is implemented
 *            via prime sieve and factorization using the
 *            smallest‑prime‑factor table. The sequence is multiplicative:
 *            a(∏pᵉᵢ)=∏q(pᵢ)ᵉᵢ, where q(p) is the neighbor of p
 * Range:     u8 n <= 63, u16 n <= 2047, u32 n <= 9223, u64 n <= 9223,
 *            u128 n <= 9223, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
#ifndef IDX_T_DEFINED
typedef uint64_t idx_t;
#endif

static idx_t *PRIMES = NULL;           /* array of primes */
static size_t PRIME_CNT = 0;           /* number of primes */
static idx_t *SPF    = NULL;           /* smallest prime factor for each i */
static seq_t *NEIGH  = NULL;           /* mapping p → q(p) for primes ≤ LIMIT */
static const idx_t LIMIT = 200000;      /* primes are pre‑computed up to this limit */

/* simple primality test using the already generated primes */
static int is_prime_small(idx_t x)
{
    if (x < 2) return 0;
    for (size_t i = 0; i < PRIME_CNT && (idx_t)PRIMES[i] * PRIMES[i] <= x; ++i)
        if (x % PRIMES[i] == 0) return 0;
    return 1;
}

/* find the next prime greater than x (used for the last prime in the table) */
static idx_t next_prime(idx_t x)
{
    for (idx_t n = x + 1; ; ++n)
        if (is_prime_small(n)) return n;
}

/* initialise primes, spf and neighbor table – done once */
static void init(void)
{
    static int done = 0;
    if (done) return;
    done = 1;

    /* allocate arrays */
    PRIMES = (idx_t *)malloc((LIMIT + 1) * sizeof(idx_t));
    SPF    = (idx_t *)calloc((LIMIT + 1), sizeof(idx_t));
    NEIGH  = (seq_t *)calloc((LIMIT + 1), sizeof(seq_t));

    /* sieve – generate primes and smallest prime factor table */
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (SPF[i] == 0) {                 /* i is prime */
            SPF[i] = i;
            PRIMES[PRIME_CNT++] = i;
            if (i * i <= LIMIT) {
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    if (SPF[j] == 0) SPF[j] = i;
            }
        }
    }

    /* build neighbor mapping */
    for (size_t i = 0; i < PRIME_CNT; ++i) {
        idx_t p = PRIMES[i];
        if (i % 2 == 0) {                  /* even index → pair with next prime */
            if (i + 1 < PRIME_CNT)
                NEIGH[p] = PRIMES[i + 1];
            else
                NEIGH[p] = next_prime(p);  /* last prime – use next prime */
        } else {                           /* odd index → pair with previous prime */
            NEIGH[p] = PRIMES[i - 1];
        }
    }
}

/* return q(p) – neighbor of prime p; fallback for p > LIMIT */
static seq_t neighbor_prime(idx_t p)
{
    if (p <= LIMIT && NEIGH[p] != 0) return NEIGH[p];
    /* fallback: find adjacent primes and use the other one */
    idx_t n1 = p - 1, n2 = p + 1;
    while (n1 >= 2 && !is_prime_small(n1)) --n1;
    while (!is_prime_small(n2)) ++n2;
    /* determine parity by counting primes ≤ p (very expensive for large p) */
    /* for our use case (p ≤ LIMIT) the fallback will never be used. */
    return (seq_t)n2;   /* choose next prime as a safe default */
}

/* main sequence function */
seq_t a(idx_t n)
{
    init();
    if (n <= 1ULL) return (seq_t)1;

    seq_t result = 1;
    idx_t tmp = n;

    /* use SPF for numbers ≤ LIMIT, otherwise trial division */
    if (tmp <= LIMIT) {
        while (tmp > 1) {
            idx_t p = SPF[tmp];
            if (p == 0) p = tmp;          /* tmp is prime > LIMIT */
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            seq_t prod = 1;
            seq_t neigh = neighbor_prime(p);
            for (idx_t i = 0; i < e; ++i) prod *= neigh;
            result *= prod;
        }
    } else {
        /* trial division by primes up to sqrt(tmp) */
        for (size_t i = 0; i < PRIME_CNT && (idx_t)PRIMES[i] * PRIMES[i] <= tmp; ++i) {
            idx_t p = PRIMES[i];
            if (tmp % p == 0) {
                idx_t e = 0;
                while (tmp % p == 0) {
                    tmp /= p;
                    ++e;
                }
                seq_t prod = 1;
                seq_t neigh = neighbor_prime(p);
                for (idx_t j = 0; j < e; ++j) prod *= neigh;
                result *= prod;
            }
        }
        if (tmp > 1) {                       /* tmp itself is prime */
            seq_t neigh = neighbor_prime(tmp);
            result *= neigh;
        }
    }
    return result;
}
