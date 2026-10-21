/*
 * A090333  Nonprimes with no divisors >1 that are prefixes of other divisors
 *          in binary representation.
 * https://oeis.org/A090333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 8906, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A090333: nth non‑prime whose non‑trivial divisors have no binary‑prefix
   relation. The term is found by scanning integers and rejecting those
   that violate the property. The routine memoises results so calls may
   arrive in any order. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/bitlen_seq_t.h"

static bool candidate_ok(idx_t mIdx)
{
    if (mIdx == (idx_t)1) return true;

    idx_t tmp = mIdx;
    idx_t primes[32];
    unsigned exps[32];
    size_t nf = 0;

    if ((tmp & 1) == 0) {
        unsigned c = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            ++c;
        }
        primes[nf] = 2;
        exps[nf++] = c;
    }

    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            unsigned c = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++c;
            }
            primes[nf] = p;
            exps[nf++] = c;
        }
    }
    if (tmp > 1) {
        primes[nf] = tmp;
        exps[nf]   = 1;
        ++nf;
    }

    idx_t dlist[256];
    size_t dcnt = 1;
    dlist[0] = 1;

    for (size_t i = 0; i < nf; ++i) {
        size_t cur = dcnt;
        idx_t mul = 1;
        for (unsigned e = 1; e <= exps[i]; ++e) {
            mul *= primes[i];
            for (size_t j = 0; j < cur; ++j) {
                dlist[dcnt++] = dlist[j] * mul;
                if (dcnt >= sizeof(dlist)/sizeof(dlist[0])) goto too_many;
            }
        }
    }

too_many:
    idx_t divs[256];
    unsigned blen[256];
    size_t nd = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        if (dlist[i] == (idx_t)1) continue;
        divs[nd] = dlist[i];
        blen[nd] = bitlen_seq_t((seq_t)dlist[i]);
        ++nd;
    }

    for (size_t i = 0; i < nd; ++i) {
        for (size_t j = i + 1; j < nd; ++j) {
            unsigned li = blen[i], lj = blen[j];
            if (li == lj) continue;
            if (li < lj) {
                if (((seq_t)divs[j]) >> (lj - li) == (seq_t)divs[i]) return false;
            } else {
                if (((seq_t)divs[i]) >> (li - lj) == (seq_t)divs[j]) return false;
            }
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t last_checked = 0;

    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    while (cache_len < (size_t)n) {
        ++last_checked;
        if (is_prime(last_checked)) continue;
        if (candidate_ok(last_checked)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
