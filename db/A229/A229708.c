/*
 * A229708  Least prime of maximal order mod n.
 * https://oeis.org/A229708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least prime of maximal order mod n
 * Range:     u8 n <= 431, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"
#include "../utils/factor_u64.h"

/* --------------------------------------------------------------
   Compute Carmichael λ(n) for n ≥ 1.
   -------------------------------------------------------------- */
static seq_t carmichael(uint64_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t primes[16], exps[16];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t l = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];
        seq_t lam;

        if (p == 2) {
            if (e == 1)          lam = (seq_t)1;
            else if (e == 2)     lam = (seq_t)2;
            else                 lam = (seq_t)1 << (e - 2); /* 2^(e-2) */
        } else {
            seq_t p_pow = (seq_t)1;
            for (uint64_t k = 1; k < e; ++k) p_pow *= p;   /* p^(e-1) */
            lam = p_pow * (p - 1);
        }

        seq_t g = gcd(l, lam);
        l = (l / g) * lam;
    }
    return l;
}

/* --------------------------------------------------------------
   Prime sieve up to a fixed limit, built once.
   -------------------------------------------------------------- */
static uint64_t *g_primes = NULL;
static size_t     g_prime_cnt = 0;
static const idx_t PRIME_LIMIT = 200000;      /* ample for n ≤ 10 000 */

static void init_primes(void)
{
    if (g_primes) return;            /* already built */

    bool *is_comp = (bool*)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    if (!is_comp) return;            /* unlikely */

    for (idx_t i = 2; i <= PRIME_LIMIT; ++i) is_comp[i] = false;
    for (idx_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_comp[j] = true;
        }
    }

    g_primes = (uint64_t*)malloc(PRIME_LIMIT * sizeof(uint64_t));
    if (!g_primes) { free(is_comp); return; }

    size_t cnt = 0;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            g_primes[cnt++] = i;
        }
    }
    g_prime_cnt = cnt;
    free(is_comp);
}

/* --------------------------------------------------------------
   Return a(n) for n ≥ 1.
   -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_primes();

    seq_t lam = carmichael((uint64_t)n);
    uint64_t lam_uint = (uint64_t)lam;   /* safe for n ≤ 10 000 */

    uint64_t lam_primes[16], lam_exps[16];
    size_t lam_cnt = 0;
    factor_u64(lam_uint, lam_primes, lam_exps, &lam_cnt);

    for (size_t i = 0; i < g_prime_cnt; ++i) {
        uint64_t p = g_primes[i];
        if (gcd((seq_t)p, (seq_t)n) != (seq_t)1) continue;

        bool ok = true;
        for (size_t j = 0; j < lam_cnt; ++j) {
            uint64_t q = lam_primes[j];
            idx_t exp = (idx_t)(lam_uint / q);
            seq_t r = mod_pow((seq_t)p, exp, (seq_t)n);
            if (r == (seq_t)1) { ok = false; break; }
        }

        if (ok) return (seq_t)p;
    }

    /* For n up to 10 000 this point is never reached. */
    return (seq_t)2;
}
