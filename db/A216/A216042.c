/*
 * A216042  Numbers k such that Sum_{j=1..k-1} (2*j)!/4^j is an integer.
 * https://oeis.org/A216042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = the n-th integer k such that Σ_{j=1}^{k-1} (2j)!/4^j is
 *            an integer. Computation is based on the 2‑adic valuation. For
 *            N_n = Σ_{j=1}^{n} (2j)!·4^(n-j) (k = n+1), v2(N_n) ≥ 2n ⇔
 *            Σ_{j=1}^{n} odd_j · 2^{M - s2(2j)} ≡ 0 (mod 2^M), where M =
 *            max_{1≤j≤n} s2(2j) and odd_j is the odd part of (2j)!...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   popcount_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/popcount_u64.h"

/* helper: popcount for 64‑bit */

/* sequence function */
seq_t a(idx_t n) {
    /* static state */
    static idx_t cur_n = 0;              /* last n processed (corresponds to k‑1) */
    static unsigned int cur_M = 0;       /* current maximum s₂(2j) */
    static unsigned long long S = 0;     /* running sum modulo 2^cur_M */

    /* odd part of (2j)! modulo 2^20 */
    static unsigned int *odd_mod = NULL;
    static size_t odd_cap = 0;

    /* array of found k values */
    static seq_t *ks = NULL;
    static size_t ks_cap = 0;
    static size_t ks_size = 0;

    /* initialise on first call */
    if (!ks) {
        ks_cap = 16;
        ks = (seq_t*)malloc(ks_cap * sizeof(seq_t));
        ks[0] = (seq_t)1;   /* k = 1 (n = 0) */
        ks_size = 1;
    }
    if (!odd_mod) {
        odd_cap = 16;
        odd_mod = (unsigned int*)malloc(odd_cap * sizeof(unsigned int));
        odd_mod[0] = 1;     /* odd part of 0! = 1 */
    }

    /* generate terms until we have at least n of them */
    while (ks_size < (size_t)n) {
        /* step to next n (i.e., k‑1) */
        cur_n++;

        /* compute s2(2n) */
        uint64_t two_n = (uint64_t)2 * cur_n;
        int s2 = popcount_u64(two_n);

        /* if maximum increases, shift the running sum */
        if ((unsigned int)s2 > cur_M) {
            S = (S << 1) & ((1ULL << (cur_M + 1)) - 1);
            cur_M++;
        }

        /* ensure odd_mod array has room */
        if ((size_t)cur_n >= odd_cap) {
            size_t new_cap = odd_cap * 2;
            odd_mod = (unsigned int*)realloc(odd_mod, new_cap * sizeof(unsigned int));
            odd_cap = new_cap;
        }

        /* compute odd part for n: (2n)! with all factors of 2 removed, mod 2^20 */
        unsigned long long odd = odd_mod[cur_n - 1];
        /* multiply by (2n-1) (odd) */
        odd = (odd * (2 * cur_n - 1)) & ((1ULL << 20) - 1);
        /* multiply by (2n) after removing all 2s */
        uint64_t val = (uint64_t)2 * cur_n;
        while ((val & 1ULL) == 0) val >>= 1;
        odd = (odd * val) & ((1ULL << 20) - 1);
        odd_mod[cur_n] = (unsigned int)odd;

        /* term contribution modulo 2^cur_M */
        unsigned int odd_mask = (cur_M == 0) ? 0 : ((1U << cur_M) - 1);
        unsigned int odd_part_mod = odd_mod[cur_n] & odd_mask;
        unsigned long long term = ((unsigned long long)odd_part_mod) << (cur_M - s2);
        S = (S + term) & ((1ULL << cur_M) - 1);

        /* if the sum is divisible by 2^cur_M, we found a new k */
        if (S == 0) {
            seq_t new_k = (seq_t)(cur_n + 1);
            if (ks_size >= ks_cap) {
                size_t new_cap = ks_cap * 2;
                ks = (seq_t*)realloc(ks, new_cap * sizeof(seq_t));
                ks_cap = new_cap;
            }
            ks[ks_size++] = new_k;
        }
    }

    return ks[n - 1];
}
