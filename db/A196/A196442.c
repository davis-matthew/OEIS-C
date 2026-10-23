/*
 * A196442  a(1) = a(2) = 1; for n >= 2, a(n) is the product of number k <= n
 *          such that GCQ_A(n, k) >= 2 (see definition in comments).
 * https://oeis.org/A196442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{k=1..n, GCQ_A(n,k) >= 2} k (with a(1) = a(2) =
 *            1)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* ---- global data needed for all calls ---- */
#define MAX_N 10000   /* assumed maximum n needed (<=10 000) */

static int prime_of_value[MAX_N + 1];   /* prime_of_value[p] = index in primes[] or -1 */
static idx_t primes[1229];             /* primes up to MAX_N (actual count <=1229) */
static size_t prime_count;             /* number of primes < = MAX_N */
static idx_t spf[MAX_N + 1];           /* smallest prime factor for every number */

/* initialise static data once */
static void init_globals(void) {
    static int initialized = 0;
    if (initialized) return;
    initialized = 1;

    for (size_t i = 0; i <= MAX_N; ++i) {
        prime_of_value[i] = -1;
        spf[i] = 0;
    }

    /* Sieve to fill spf and primes */
    for (idx_t i = 2; i <= MAX_N; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = i;
            prime_of_value[i] = prime_count;
            primes[prime_count++] = i;
            for (idx_t j = i * i; j <= MAX_N; j += i) {
                if (spf[j] == 0) spf[j] = i;
            }
        }
    }
}

/* -------------------------------------------------------------------------- */
/* Main sequence function ----------------------------------------------------- */
seq_t a(idx_t n) {
    /* basic cases */
    if (n <= 2) return (seq_t)1;

    init_globals();

    /* array of exponents for current lcm; size = prime_count */
    int exp[1229];
    memset(exp, 0, prime_count * sizeof(int));

    idx_t L_value = 1;          /* current lcm value (capped at MAX_N) */
    int L_exceeds = 0;          /* becomes 1 once lcm exceeds MAX_N */

    seq_t prod = (seq_t)1;

    /* iterate over k = 2 .. n */
    for (idx_t k = 2; k <= n; ++k) {
        /* Update lcm with k if k does not divide n */
        if (n % k != 0) {
            idx_t temp = k;
            while (temp > 1) {
                idx_t p = spf[temp];
                int e = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++e;
                }
                int pi = prime_of_value[p];
                if (e > exp[pi]) {
                    int delta = e - exp[pi];
                    exp[pi] = e;
                    if (!L_exceeds) {
                        for (int t = 0; t < delta; ++t) {
                            if (L_value > MAX_N / p) {
                                L_exceeds = 1;
                                break;
                            }
                            L_value *= p;
                        }
                    }
                }
                if (L_exceeds) {
                    /* skip further updates, L_value not needed */
                    while (temp > 1) {
                        idx_t p2 = spf[temp];
                        while (temp % p2 == 0) temp /= p2;
                    }
                    break;
                }
            }
        }

        /* Determine if GCQ_A(k) >= 2 (i.e., excluded) */
        int excluded = 0;
        if (k < 3) {
            excluded = 1;          /* k = 1 or 2 are always excluded */
        } else {
            if (L_exceeds || L_value > k) {
                excluded = 0;
            } else {
                excluded = ((k % L_value) == 0) ? 1 : 0;
            }
        }

        if (!excluded) {
            prod *= (seq_t)k;
        }
    }

    return prod;
}
