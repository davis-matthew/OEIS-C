/*
 * A056479  Number of primitive (aperiodic) palindromic structures using a
 *          maximum of five different symbols.
 * https://oeis.org/A056479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * A056470(n/d) where A056470(m) =
 *            Sum_{k=1..5} S(ceil(m/2), k) and S(n, k) are Stirling numbers of
 *            the second kind
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 32, u64 n <= 60, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of primitive palindromic structures over at most five symbols. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_N 20000                 /* safe upper bound for n */
#define HALF_SIZE ((MAX_N / 2) + 2)  /* array size for S[] */

static bool initialized = false;

static seq_t f[MAX_N + 1];          /* f[n] = total palindromes (not necessarily primitive) */
static int   mu[MAX_N + 1];          /* Möbius function */
static seq_t S[HALF_SIZE][6];        /* Stirling numbers S[n][k], k=0..5 */

/* initialise tables */
static void init(void) {
    /* ----------------------------------
       Compute Möbius function up to MAX_N
       ---------------------------------- */
    int lp[MAX_N + 1] = {0};           /* least prime factor */
    int primes[MAX_N / 10 + 1];
    int pcnt = 0;
    mu[1] = 1;
    for (int i = 2; i <= MAX_N; ++i) {
        if (lp[i] == 0) {
            lp[i] = i;
            primes[pcnt++] = i;
            mu[i] = -1;
        }
        for (int j = 0; j < pcnt && primes[j] <= lp[i] && i * primes[j] <= MAX_N; ++j) {
            int p = primes[j];
            int ip = i * p;
            lp[ip] = p;
            if (i % p == 0) {
                mu[ip] = 0;
            } else {
                mu[ip] = -mu[i];
            }
        }
    }

    /* ----------------------------------
       Compute Stirling numbers S[n][k] (k ≤ 5)
       ---------------------------------- */
    S[0][0] = 1u;
    for (int k = 1; k <= 5; ++k) S[0][k] = 0u;

    for (int n = 1; n < HALF_SIZE; ++n) {
        for (int k = 1; k <= 5; ++k) {
            S[n][k] = (seq_t)k * S[n - 1][k] + S[n - 1][k - 1];
        }
        /* zero out unused entries (already zero by static storage) */
    }

    /* ----------------------------------
       Compute total palindromes f[n] for n up to MAX_N
       ---------------------------------- */
    f[0] = 0u;  /* not used; a(0) handled separately */
    for (int n = 1; n <= MAX_N; ++n) {
        int h = (n + 1) / 2;        /* ceil(n/2) */
        seq_t sum = 0u;
        for (int k = 1; k <= 5; ++k) {
            sum += S[h][k];
        }
        f[n] = sum;
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 1u;           /* a(0) = 1 */

    if (!initialized) { init(); initialized = true; }

    seq_t sum_pos = 0u;   /* contributions with mu = +1 */
    seq_t sum_neg = 0u;   /* contributions with mu = -1 */

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            if (mu[d1] == 1)      sum_pos += f[n / d1];
            else if (mu[d1] == -1) sum_neg += f[n / d1];

            if (d1 != d2) {
                if (mu[d2] == 1)      sum_pos += f[n / d2];
                else if (mu[d2] == -1) sum_neg += f[n / d2];
            }
        }
    }

    /* result = sum_pos - sum_neg; result is guaranteed non‑negative */
    return sum_pos - sum_neg;
}
