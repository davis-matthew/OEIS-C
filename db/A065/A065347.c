/*
 * A065347  Positions of zeros in A065344, i.e., binomial(2n,n) mod
 *          ((n+1)*(n+2)) = 0.
 * https://oeis.org/A065347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k such that binomial(2k,k) is divisible
 *            by (k+1)(k+2). The implementation tests each candidate k by
 *            checking that the prime–power exponents of (k+1)(k+2) are all
 *            less than or equal to the corresponding exponents in
 *            binomial(2k,k) using Legendre’s formula
 * Range:     u8 n <= 220, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   vp_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/vp_factorial.h"

#define SIEVE_LIMIT 1000000u   /* large enough for all requested terms */

/* static data used by the algorithm ------------------------------------ */
static uint32_t *spf = NULL;         /* smallest prime factor array */
static size_t      last_k = 0;        /* largest k that was examined */
static seq_t      *zero_pos = NULL;   /* array of found zero positions */
static size_t      zero_len = 0;      /* number stored in zero_pos */
static size_t      zero_cap = 0;      /* allocated capacity of zero_pos */

/* ---- sieve and factorisation ------------------------------------------ */
static void init_sieve(void)
{
    if (spf != NULL) return;                 /* already initialised */

    spf = (uint32_t*)malloc((SIEVE_LIMIT + 1) * sizeof(uint32_t));
    if (!spf) abort();

    for (uint32_t i = 0; i <= SIEVE_LIMIT; ++i) spf[i] = 0;

    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == 0) {                   /* i is prime */
            spf[i] = i;
            if ((uint64_t)i * i <= SIEVE_LIMIT) {
                for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
}

struct prime_exp { uint32_t p; int exp; };

static void factor_add(uint32_t x, struct prime_exp *need, int *need_len)
{
    while (x > 1) {
        uint32_t p = spf[x];
        int cnt = 0;
        while (x % p == 0) { x /= p; ++cnt; }
        int i = 0;
        while (i < *need_len && need[i].p != p) ++i;
        if (i < *need_len)
            need[i].exp += cnt;
        else {
            need[*need_len].p = p;
            need[*need_len].exp = cnt;
            ++(*need_len);
        }
    }
}

/* ---- dynamic array of zero positions --------------------------------- */
static void ensure_zero_capacity(size_t need)
{
    if (zero_cap >= need) return;
    size_t newcap = zero_cap == 0 ? 16 : zero_cap * 2;
    while (newcap < need) newcap *= 2;
    seq_t *tmp = (seq_t*)realloc(zero_pos, newcap * sizeof(seq_t));
    if (!tmp) abort();
    zero_pos = tmp;
    zero_cap = newcap;
}

/* ---- generate zeros until we have at least 'target' terms --------------- */
static void compute_zeros_until(size_t target)
{
    if (zero_len >= target) return;

    init_sieve();

    idx_t k = last_k + 1;
    while (zero_len < target) {
        struct prime_exp need[32];
        int need_len = 0;

        factor_add((uint32_t)(k + 1), need, &need_len);
        factor_add((uint32_t)(k + 2), need, &need_len);

        bool ok = true;
        for (int i = 0; i < need_len; ++i) {
            uint32_t p = need[i].p;
            unsigned int req = need[i].exp;
            unsigned int vp = vp_factorial(2 * k, p) - 2 * vp_factorial(k, p);
            if (vp < req) { ok = false; break; }
        }
        if (ok) {
            ensure_zero_capacity(zero_len + 1);
            zero_pos[zero_len++] = (seq_t)k;
        }
        ++k;
    }
    last_k = k - 1;           /* last k that was examined */
}

/* ---- public API ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 1, but safer */
    compute_zeros_until((size_t)n);
    return zero_pos[n - 1];
}
