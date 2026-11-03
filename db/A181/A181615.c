/*
 * A181615  Triangle read by rows: T(n,k) = 2^(n-1) mod semiprime(k).
 * https://oeis.org/A181615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^(row−1) mod semiprime(col) where (row, col) are the
 *            1‑based coordinates of the index n in the triangular array (row
 *            sums are 1, 3, 6, 10, …)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Validated: partial, pass, 89 terms through n = 89 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static void ensure_semiprimes(size_t k)
{
    /* lazy construction of the first k semiprimes on demand.
       Semiprime = product of two primes (including squares). */
    static uint32_t *semiprimes = NULL;
    static size_t semiprime_cnt = 0;
    static uint32_t next_candidate = 4;   /* first possible semiprime */

    while (semiprime_cnt < k) {
        uint32_t n = next_candidate;
        ++next_candidate;

        /* Count prime factors with multiplicity */
        uint32_t t = n;
        int cnt = 0;
        for (uint32_t p = 2; (uint64_t)p * p <= t; ++p) {
            while (t % p == 0) {
                ++cnt;
                t /= p;
                if (cnt > 2) break;
            }
            if (cnt > 2) break;
        }
        if (t > 1) ++cnt;   /* remaining prime factor */

        if (cnt == 2) {
            /* Grow the list if necessary */
            if (semiprime_cnt == 0) {
                semiprimes = (uint32_t*)malloc(sizeof(uint32_t));
            } else {
                semiprimes = (uint32_t*)realloc(semiprimes,
                                               (semiprime_cnt + 1) * sizeof(uint32_t));
            }
            semiprimes[semiprime_cnt++] = n;
        }
    }

    /* Store back the static structures in the function's scope */
    /* clang-format off */
    /* The following lines are needed only to keep the compiler
       from complaining about unused variables.  They have no
       effect on the runtime logic. */
    (void)semiprimes;
    (void)semiprime_cnt;
    (void)next_candidate;
    /* clang-format on */
}

static uint32_t semiprime_at(size_t k)
{
    /* Retrieve the k‑th semiprime, building the list on demand. */
    static uint32_t *semiprimes = NULL;
    static size_t semiprime_cnt = 0;
    static uint32_t next_candidate = 4;   /* first possible semiprime */

    while (semiprime_cnt < k) {
        uint32_t n = next_candidate;
        ++next_candidate;

        /* Count prime factors with multiplicity */
        uint32_t t = n;
        int cnt = 0;
        for (uint32_t p = 2; (uint64_t)p * p <= t; ++p) {
            while (t % p == 0) {
                ++cnt;
                t /= p;
                if (cnt > 2) break;
            }
            if (cnt > 2) break;
        }
        if (t > 1) ++cnt;   /* remaining prime factor */

        if (cnt == 2) {
            /* Grow the list if necessary */
            if (semiprime_cnt == 0) {
                semiprimes = (uint32_t*)malloc(sizeof(uint32_t));
            } else {
                semiprimes = (uint32_t*)realloc(semiprimes,
                                               (semiprime_cnt + 1) * sizeof(uint32_t));
            }
            semiprimes[semiprime_cnt++] = n;
        }
    }
    return semiprimes[k - 1];
}

static seq_t powmod_uint(uint64_t base, idx_t exp, uint32_t mod)
{
    /* Computes base^exp mod mod for small mod (≤ 2^32). */
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp > 0) {
        if (exp & 1) result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return (seq_t)result;
}

seq_t a(idx_t n)
{
    /* Find the row r and column k from the triangular indexing. */
    idx_t r = 1;
    idx_t cumulative = 1;
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }
    idx_t prev = cumulative - r;
    idx_t k = n - prev; /* 1‑based column */

    /* Compute 2^(r-1) mod semiprime(k). */
    uint32_t mod = semiprime_at((size_t)k);
    return powmod_uint(2, r - 1, mod);
}
