/*
 * A023029  Number of partitions of n into 9 unordered relatively prime parts.
 * https://oeis.org/A023029
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * p9(n/d) * where p9(m) is the number of
 *            unordered partitions of m into exactly 9 parts * (p9(m) =
 *            part_leq9[m-9] for m ≥ 9, with part_leq9[s] counting partitions
 *            of * s into parts 1..9 with unlimited multiplicity)
 * Range:     u8 n <= 26, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Precomputation limit – must be ≥ the largest n the tests will use.  */
#define MAX_N 200000

/* Arrays used by the routine */
static idx_t part_leq9[MAX_N + 1];   /* partitions of s into parts 1..9   */
static int   mu[MAX_N + 1];          /* Möbius function                   */
static bool  initialized = false;    /* one‑time initialization flag      */

/* One‑time precomputation: Möbius values and part_leq9 array */
static void init_sequence(void)
{
    if (initialized) return;
    initialized = true;

    /* Initialise arrays */
    for (size_t i = 0; i <= MAX_N; ++i) {
        mu[i] = 0;
        part_leq9[i] = 0;
    }
    mu[1] = 1;

    /* Linear sieve to compute μ up to MAX_N */
    static unsigned int primes[MAX_N / 5];
    size_t sz = 0;
    static unsigned int lp[MAX_N + 1];   /* lowest prime factor */

    for (size_t i = 2; i <= MAX_N; ++i) {
        if (lp[i] == 0) {
            lp[i] = (unsigned int)i;
            primes[sz++] = (unsigned int)i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < sz && primes[j] <= lp[i] &&
             (size_t)primes[j] * i <= MAX_N; ++j)
        {
            size_t v = primes[j] * i;
            lp[v] = primes[j];
            if (primes[j] == lp[i])
                mu[v] = 0;
            else
                mu[v] = -mu[i];
        }
    }

    /* Compute number of partitions of s into parts 1..9 (unlimited multiplicity) */
    part_leq9[0] = 1;
    const size_t limit = MAX_N > 9 ? MAX_N - 9 : 0;
    for (unsigned int coin = 1; coin <= 9; ++coin) {
        for (size_t s = coin; s <= limit; ++s) {
            part_leq9[s] += part_leq9[s - coin];
        }
    }
}

/* Helper: partitions of m into exactly 9 parts */
static inline seq_t partitions9(idx_t m)
{
    return (m >= 9) ? (seq_t)part_leq9[(size_t)(m - 9)] : (seq_t)0;
}

/* Main routine: OEIS sequence A023029 */
seq_t a(idx_t n)
{
    if (n < 9) return (seq_t)0;   /* offset is 9 */
    init_sequence();

    seq_t result = 0;
    /* Enumerate divisors d of n up to sqrt(n) without overflow */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* divisor d */
            if (mu[d] == 1) {
                result += partitions9(n / d);
            } else if (mu[d] == -1) {
                result -= partitions9(n / d);
            }

            /* paired divisor n/d, avoid double counting when d == n/d */
            idx_t d2 = n / d;
            if (d2 != d && d2 <= MAX_N) {
                if (mu[d2] == 1) {
                    result += partitions9(d);
                } else if (mu[d2] == -1) {
                    result -= partitions9(d);
                }
            }
        }
    }
    return result;
}
