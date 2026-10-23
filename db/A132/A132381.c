/*
 * A132381  Number of partitions of n with exactly one prime number.
 * https://oeis.org/A132381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p prime ≤ n} Σ_{k=1}^{⌊n/p⌋} P_nonprime(n - k·p) where
 *            P_nonprime(m) is the number of partitions of m into parts that
 *            are 1 or composite (i.e., not prime)
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 171, u64 n <= 565,
 *            u128 n <= 1970, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Function prototype required by the build system */
seq_t a(idx_t n);

/* ---------- Static state ------------------------------------------- */
static seq_t *dp          = NULL;   /* partitions into non‑prime parts */
static bool   *is_prime_arr = NULL;/* sieve of Eratosthenes */
static idx_t  *primes_arr  = NULL;  /* list of primes up to current size */
static size_t  prime_cnt   = 0;     /* number of primes in primes_arr   */
static idx_t   max_n_cached = 0;    /* largest n for which data is ready */

/* ---------- Helper to rebuild tables up to n ----------------------- */
static void recompute(idx_t n)
{
    /* Allocate new dp array */
    size_t N = (size_t)n;
    seq_t *dp_new = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!dp_new) return;        /* allocation failed – nothing to do */
    dp_new[0] = 1;               /* empty partition */

    /* ---------- Sieve of Eratosthenes -------------------------------- */
    bool *prime_new = (bool*)malloc((N + 1) * sizeof(bool));
    if (!prime_new) { free(dp_new); return; }
    for (size_t i = 0; i <= N; ++i) prime_new[i] = true;
    if (N >= 0) prime_new[0] = false;
    if (N >= 1) prime_new[1] = false;
    for (size_t i = 2; i * i <= N; ++i)
        if (prime_new[i])
            for (size_t j = i * i; j <= N; j += i)
                prime_new[j] = false;

    /* ---------- Collect primes ---------------------------------------- */
    idx_t *primes_new = (idx_t*)malloc((N + 1) * sizeof(idx_t));
    if (!primes_new) { free(dp_new); free(prime_new); return; }
    size_t pc = 0;
    for (size_t i = 2; i <= N; ++i)
        if (prime_new[i])
            primes_new[pc++] = (idx_t)i;

    /* ---------- Free old tables --------------------------------------- */
    if (dp)          free(dp);
    if (is_prime_arr) free(is_prime_arr);
    if (primes_arr)  free(primes_arr);

    /* ---------- Assign new tables ------------------------------------ */
    dp            = dp_new;
    is_prime_arr  = prime_new;
    primes_arr    = primes_new;
    prime_cnt     = pc;
    max_n_cached  = n;

    /* ---------- Compute partitions into non‑prime parts ---------------- */
    for (size_t s = 1; s <= N; ++s) {
        if (!prime_new[s]) {                  /* s is 1 or composite   */
            for (size_t i = s; i <= N; ++i) {
                dp[i] += dp[i - s];
            }
        }
    }
}

/* ---------- Main sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 0) return 0;                     /* undefined for n<1 */

    if (n > max_n_cached) recompute(n);

    seq_t res = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes_arr[i];
        if (p > n) break;
        /* sum over all positive multiples of p */
        for (idx_t mp = p; mp <= n; mp += p) {
            res += dp[n - mp];
        }
    }
    return res;
}
