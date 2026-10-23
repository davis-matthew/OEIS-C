/*
 * A139556  a(n) = sum of the prime-powers (including 1) that each are <= n
 *          and are coprime to n.
 * https://oeis.org/A139556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the prime‑powers (including 1) that are <= n and
 *            are coprime to n
 * Range:     u8 n <= 36, u16 n <= 880, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* -------------------------------------------------------------------------- */
/* Euclidean GCD for 64‑bit unsigned integers                                 */
/* -------------------------------------------------------------------------- */
static inline uint64_t gcd_ull(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* -------------------------------------------------------------------------- */
/* Prime‑power table (works up to a given limit)                              */
/* -------------------------------------------------------------------------- */
static uint64_t *prime_powers = NULL;
static size_t   pp_cnt      = 0;
static size_t   pp_cap      = 0;
static idx_t    pp_limit    = 0;

/* Append a value to the table, growing it if necessary */
static void pp_push(uint64_t val)
{
    if (pp_cnt == pp_cap) {
        pp_cap = pp_cap ? pp_cap * 2 : 4;
        prime_powers = (uint64_t*)realloc(prime_powers, pp_cap * sizeof(uint64_t));
    }
    prime_powers[pp_cnt++] = val;
}

/* Build all prime powers <= L (inclusive) using a simple sieve */
static void build_pp_for_limit(idx_t L)
{
    /* free previous table */
    free(prime_powers);
    prime_powers = NULL;
    pp_cnt = 0;
    pp_cap = 0;
    pp_limit = 0;

    size_t n = (size_t)L;
    bool *sieve = (bool*)malloc((n + 1) * sizeof(bool));
    if (!sieve) return;                 /* allocation failure – unlikely */

    for (size_t i = 0; i <= n; ++i) sieve[i] = false;
    for (size_t i = 2; i * i <= n; ++i) {
        if (!sieve[i]) {
            for (size_t j = i * i; j <= n; j += i) sieve[j] = true;
        }
    }

    /* 1 counts as a prime power */
    pp_push(1ULL);

    for (size_t p = 2; p <= n; ++p) {
        if (sieve[p]) continue;         /* not prime */
        uint64_t val = p;
        while (val <= (uint64_t)L) {
            pp_push(val);
            if (val > (uint64_t)L / p) break;   /* avoid overflow */
            val *= p;
        }
    }

    free(sieve);
    pp_limit = L;
}

/* Ensure the table is valid up to at least n */
static void ensure_pp(idx_t n)
{
    if (n <= pp_limit) return;
    build_pp_for_limit(n);
}

/* -------------------------------------------------------------------------- */
/* Main sequence function                                                     */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_pp(n);

    seq_t sum = 0;
    for (size_t i = 0; i < pp_cnt; ++i) {
        uint64_t m = prime_powers[i];
        if (m > (uint64_t)n) break;
        if (gcd_ull(m, (uint64_t)n) == 1) {
            sum += (seq_t)m;
        }
    }
    return sum;
}
