/*
 * A014662  Primes p such that order of 2 mod p (=A007733(p)) is even.
 * https://oeis.org/A014662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, is_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A014662: primes p with even multiplicative order of 2 modulo p (excluding 2) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/powmod64.h"

/* Cached list of primes with even order of 2 modulo p */
static uint64_t *prop = NULL;
static size_t prop_cnt = 0;
static size_t prop_cap = 0;
static idx_t last_checked = 1;   /* start from 1 */

/* Return true iff the multiplicative order of 2 modulo odd prime p is even */
static bool is_even_order(uint64_t p)
{
    uint64_t phi = p - 1;

    /* Factor φ(p) */
    uint64_t pf[64]  = {0};
    uint64_t exp[64] = {0};
    size_t fac_cnt = 0;
    factor_u64(phi, pf, exp, &fac_cnt);

    uint64_t ord = phi;

    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t q = pf[i];
        while (ord % q == 0) {
            if (powmod64(2, ord / q, p) == 1)
                ord /= q;
            else
                break;
        }
    }
    return (ord & 1) == 0;
}

/* Expand the cached list until it contains at least 'need' primes */
static void ensure_prop_count(idx_t need)
{
    while ((idx_t)prop_cnt < need) {
        /* Find next prime */
        do {
            ++last_checked;
        } while (!is_prime(last_checked));

        /* Skip p = 2 */
        if (last_checked == 2) {
            continue;
        }

        if (is_even_order(last_checked)) {
            if (prop_cnt == prop_cap) {
                prop_cap = prop_cap ? prop_cap * 2 : 8;
                uint64_t *tmp = (uint64_t *)realloc(prop, prop_cap * sizeof(uint64_t));
                if (!tmp) abort();
                prop = tmp;
            }
            prop[prop_cnt++] = last_checked;
        }
    }
}

/* Main OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* behaviour below offset is irrelevant */
    ensure_prop_count(n);
    return (seq_t)prop[n - 1];
}
