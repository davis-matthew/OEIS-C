/*
 * A062790  Moebius transform of the cototient function A051953.
 * https://oeis.org/A062790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 427, u16 n <= 9910, u32 n <= 9910, u64 n <= 9910,
 *            u128 n <= 9910, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{d | n} (n/d - φ(n/d)) * μ(d)
 * μ(d) is the Möbius function, φ is Euler's totient.
 * This is the Möbius transform of the cototient function A051953.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

/* ---------- Prime table ------------------------------------------- */
static uint32_t *prime_list = NULL;      /* array of discovered primes          */
static size_t   prime_cnt  = 0;           /* number of primes currently stored   */
static size_t   prime_cap  = 0;           /* current allocation capacity         */
static idx_t    prime_limit = 1;          /* highest integer already sieved      */

/* Ensure capacity for at least 'need' primes. */
static void ensure_cap(size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t newcap = (*cap == 0) ? 8 : (*cap << 1);
        if (newcap < need) newcap = need;
        uint32_t *tmp = (uint32_t *)realloc(prime_list, newcap * sizeof(uint32_t));
        if (!tmp) exit(EXIT_FAILURE);
        prime_list = tmp;
        *cap = newcap;
    }
}

/* Sieve primes up to 'limit' (inclusive) and store them in prime_list. */
static void ensure_primes(idx_t limit)
{
    if (prime_limit >= limit) return;           /* already enough */

    for (idx_t n = prime_limit + 1; n <= limit; ++n) {
        if (n < 2) continue;                    /* skip 0 and 1 */
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_list[i];
            if ((idx_t)p * (idx_t)p > n) break;
            if (n % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            ensure_cap(&prime_cap, prime_cnt + 1);
            prime_list[prime_cnt++] = (uint32_t)n;
        }
    }
    prime_limit = limit;
}

/* ---------- Divisor enumeration ----------------------------------- */
static void dfs_div(
    int                idx,
    idx_t              cur_d,
    bool               squarefree,
    int                pc,                /* number of primes with exponent 1 */
    const uint32_t    *fac,
    const int         *exp,
    int                k,
    int                *chosen,
    seq_t             *sum,
    idx_t              n)
{
    if (idx == k) {                           /* divisor complete          */
        int mu = 0;
        if (squarefree) mu = (pc & 1) ? -1 : 1;

        if (mu) {
            idx_t ndp = n / cur_d;            /* n/d                       */
            seq_t phi = (seq_t)ndp;           /* start with n/d             */
            for (int i = 0; i < k; ++i) {
                int rem = exp[i] - chosen[i];
                if (rem > 0) {
                    uint32_t p = fac[i];
                    /* φ(p^e) = p^e - p^(e-1) = p^(e-1)*(p-1) */
                    phi = phi / p * (p - 1);
                }
            }
            /* f(n/d) = n/d - φ(n/d) = ndp - phi */
            seq_t diff = ndp - phi;
            if (mu == -1) diff = -diff;
            *sum += diff;
        }
        return;
    }

    uint32_t p  = fac[idx];
    idx_t    pw = 1;
    for (int e = 0; e <= exp[idx]; ++e) {
        chosen[idx] = e;
        bool nsf = squarefree && (e < 2);
        int npc  = pc + (e == 1 ? 1 : 0);
        dfs_div(idx + 1, cur_d * pw, nsf, npc, fac, exp, k, chosen, sum, n);
        pw *= p;
    }
}

/* ---------- Main sequence function --------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* sequence is 1‑based */

    /* 1. Build primes up to √n. */
    idx_t limit = (idx_t)(sqrt((double)n)) + 1;
    ensure_primes(limit);

    /* 2. Factor n. */
    uint32_t fac[16];
    int      exp[16];
    int      k = 0;
    idx_t    tmp = n;
    for (size_t i = 0; i < prime_cnt && (idx_t)prime_list[i] * (idx_t)prime_list[i] <= tmp; ++i) {
        uint32_t p = prime_list[i];
        if (tmp % p == 0) {
            int c = 0;
            while (tmp % p == 0) { tmp /= p; ++c; }
            fac[k] = p;
            exp[k] = c;
            ++k;
        }
    }
    if (tmp > 1) {                    /* remaining prime factor > √n */
        fac[k] = (uint32_t)tmp;
        exp[k] = 1;
        ++k;
    }

    /* 3. Enumerate all divisors and accumulate the sum. */
    int chosen[16] = {0};
    seq_t sum = 0;
    dfs_div(0, (idx_t)1, true, 0, fac, exp, k, chosen, &sum, n);

    return sum;
}
