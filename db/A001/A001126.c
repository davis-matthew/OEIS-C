/*
 * A001126  Primes with 7 as smallest primitive root.
 * https://oeis.org/A001126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- a(n) is the n-th prime p whose smallest primitive root
 *            is 7 (none of 2,3,5,6 is a primitive root mod p, but 7 is); p-1
 *            factored with an SPF sieve
 * Range:     u8 n <= 3, u16 n <= 466, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prim_root, pmod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prim_root.h"
#include "../utils/pmod_pow.h"

static uint32_t *spf_tab = NULL;
static uint64_t spf_lim = 0;

static void build_spf(uint64_t L) {
    uint64_t i, j;
    free(spf_tab);
    spf_tab = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    for (i = 2; i <= L; i++) {
        if (spf_tab[i] == 0) {
            for (j = i; j <= L; j += i)
                if (spf_tab[j] == 0) spf_tab[j] = (uint32_t)i;
        }
    }
    spf_lim = L;
}

static uint64_t *terms = NULL;
static idx_t nterms = 0, cap_terms = 0;
static uint64_t next_p = 3;

seq_t a(idx_t n) {
    static const uint64_t smaller[4] = {2, 3, 5, 6};
    while (nterms < n) {
        uint64_t p = next_p++;
        if (p > spf_lim) {
            uint64_t L = spf_lim ? spf_lim * 2 : ((uint64_t)1 << 20);
            while (L < p) L *= 2;
            build_spf(L);
        }
        if (spf_tab[p] != p) continue;
        if (p <= 7) continue;
        {
            uint64_t fac[64];
            int nf = 0, i;
            bool ok = true;
            uint64_t m = p - 1;
            while (m > 1) {
                uint64_t q = spf_tab[m];
                fac[nf++] = q;
                while (m % q == 0) m /= q;
            }
            if (!is_prim_root(7, p, fac, nf)) continue;
            for (i = 0; i < 4; i++) {
                if (is_prim_root(smaller[i], p, fac, nf)) { ok = false; break; }
            }
            if (!ok) continue;
            if (nterms == cap_terms) {
                idx_t nc = cap_terms ? cap_terms * 2 : 1024;
                uint64_t *t = (uint64_t *)realloc(terms, (size_t)nc * sizeof(uint64_t));
                terms = t;
                cap_terms = nc;
            }
            terms[nterms++] = p;
        }
    }
    return (seq_t)terms[n - 1];
}
