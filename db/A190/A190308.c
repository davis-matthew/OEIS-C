/*
 * A190308  Numbers k such that tau(4k-1) = tau(4k+1) = tau(6k-1) = tau(6k+1)
 *          where tau(k) = A000005(k).
 * https://oeis.org/A190308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by searching for the n‑th integer k such that *
 *            τ(4k-1) = τ(4k+1) = τ(6k-1) = τ(6k+1). The function builds a
 *            prime * sieve on demand and counts divisors by trial division.
 *            Results are * memoised so repeated calls with arbitrary n are
 *            fast. The algorithm * runs in O(k log k) time...
 * Range:     u8 n <= 10, u16 n <= 1612, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime infrastructure ------------------------------------ */
static uint64_t *prime_list = NULL;   /* primes in ascending order       */
static size_t     prime_cap  = 0;     /* allocated capacity               */
static size_t     prime_cnt  = 0;     /* number of primes stored          */

/* Extend prime_list to contain all primes ≤ limit. */
static void extend_primes_to(uint64_t limit)
{
    if (prime_list == NULL) {
        prime_cap = 128;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (prime_list == NULL) exit(1);
    }
    if (prime_cnt == 0) {
        prime_list[prime_cnt++] = 2;
    }

    uint64_t start = (prime_cnt == 1 ? 3 : prime_list[prime_cnt - 1] + 2);
    for (uint64_t cand = start; cand <= limit; cand += 2) {
        bool composite = false;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > cand) break;
            if (cand % p == 0) {
                composite = true;
                break;
            }
        }
        if (!composite) {
            if (prime_cnt >= prime_cap) {
                prime_cap = prime_cap * 2;
                prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
                if (prime_list == NULL) exit(1);
            }
            prime_list[prime_cnt++] = cand;
        }
    }
}

/* ---------- Divisor count ------------------------------------------ */
static uint64_t divisor_count_u64(uint64_t n)
{
    if (n <= 1) return 1;

    /* Ensure primes up to sqrt(n) are available. */
    extend_primes_to(n);

    uint64_t result = 1;
    uint64_t m = n;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= m; ++i) {
        uint64_t p = prime_list[i];
        if (m % p == 0) {
            unsigned exp = 0;
            do {
                m /= p;
                ++exp;
            } while (m % p == 0);
            result *= (exp + 1);
        }
    }
    if (m > 1)  /* remaining prime factor */
        result *= 2;
    return result;
}

/* ---------- Sequence storage ---------------------------------------- */
static uint64_t *k_vals = NULL;  /* memoised k values, stored as 64‑bit */
static size_t    k_cap  = 0;
static size_t    k_len  = 0;
static uint64_t  next_k = 1;      /* next candidate k to test */

/* ---------- Main function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* behavior below offset irrelevant */

    while (k_len < n) {
        uint64_t k = next_k++;
        uint64_t v1 = 4 * k - 1;
        uint64_t v2 = 4 * k + 1;
        uint64_t v3 = 6 * k - 1;
        uint64_t v4 = 6 * k + 1;

        uint64_t tau1 = divisor_count_u64(v1);
        if (divisor_count_u64(v2) != tau1) continue;
        if (divisor_count_u64(v3) != tau1) continue;
        if (divisor_count_u64(v4) != tau1) continue;

        /* Store found k. */
        if (k_len >= k_cap) {
            size_t new_cap = k_cap ? k_cap << 1 : 16;
            uint64_t *tmp = (uint64_t *)realloc(k_vals, new_cap * sizeof(uint64_t));
            if (tmp == NULL) exit(1);
            k_vals = tmp;
            k_cap  = new_cap;
        }
        k_vals[k_len++] = k;
    }
    return (seq_t)k_vals[n - 1];
}
