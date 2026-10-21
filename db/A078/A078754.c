/*
 * A078754  Numbers k such that for all m>k there are more than n primes
 *          between m and 2m (inclusive).
 * https://oeis.org/A078754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = max{ m | π(2m) – π(m‑1) ≤ n } 
 * where π(x) is the number of primes ≤ x. 
 * For all m > a(n) the number of primes in [m, 2m] exceeds n. 
 * The code builds a sieve once and answers each query in O(1). 
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* --- static data --------------------------------------------------- */
    static int initialized = 0;
    static uint8_t *is_prime = NULL;      /* sieve: 1 = prime, 0 = composite */
    static uint32_t *pi = NULL;            /* π(x): prefix prime counts      */
    static uint32_t *ans = NULL;           /* ans[n] = largest m with count≤n */
    static uint32_t ans_size = 0;          /* number of entries in ans       */
    const uint32_t M_MAX = 2000000;        /* sufficiently large m          */
    const uint32_t LIMIT = 2 * M_MAX + 2;  /* sieve limit: at least 2*M_MAX */

    if (!initialized) {
        /* ----- build sieve ------------------------------------------- */
        is_prime = (uint8_t *)malloc(LIMIT);
        if (!is_prime) return (seq_t)0;
        for (uint32_t i = 0; i < LIMIT; ++i) is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;
        for (uint32_t p = 2; (uint64_t)p * p < LIMIT; ++p) {
            if (is_prime[p]) {
                for (uint32_t q = p * p; q < LIMIT; q += p)
                    is_prime[q] = 0;
            }
        }

        /* ----- prefix prime counts ----------------------------------- */
        pi = (uint32_t *)malloc(LIMIT * sizeof(uint32_t));
        if (!pi) { free(is_prime); return (seq_t)0; }
        pi[0] = 0;
        for (uint32_t i = 1; i < LIMIT; ++i)
            pi[i] = pi[i - 1] + is_prime[i];

        /* ----- allocate ans array ----------------------------------- */
        ans_size = pi[LIMIT - 1] + 2;   /* +2 to be safe for indexing n */
        ans = (uint32_t *)calloc(ans_size, sizeof(uint32_t));
        if (!ans) { free(is_prime); free(pi); return (seq_t)0; }

        /* ----- compute counts for each m and update ans ---------------- */
        for (uint32_t m = 1; m <= M_MAX; ++m) {
            uint32_t c = pi[2 * m] - pi[m - 1]; /* primes in [m,2m] */
            if (c < ans_size && ans[c] < m)
                ans[c] = m;
        }

        /* ----- prefix maximum over ans --------------------------------- */
        for (uint32_t i = 1; i < ans_size; ++i)
            if (ans[i] < ans[i - 1])
                ans[i] = ans[i - 1];

        initialized = 1;
    }

    /* ----- answer for given n ---------------------------------------- */
    if (n >= (idx_t)ans_size)
        return (seq_t)ans[ans_size - 1];
    return (seq_t)ans[(uint32_t)n];
}
