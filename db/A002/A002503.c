/*
 * A002503  Numbers k such that binomial(2*k,k) is divisible by (k+1)^2.
 * https://oeis.org/A002503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   search: k is a term iff for every prime p | (k+1),
 *            v_p(binomial(2k,k)) = sum_{i>=1} (floor(2k/p^i) -
 *            2*floor(k/p^i)) >= 2*v_p(k+1) (Legendre/Kummer)
 * Range:     u8 n <= 23, u16 n <= 7254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a2503_ok, a2503_vbin
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a2503_ok.h"
#include "../utils/a2503_vbin.h"

static uint64_t *a2503_terms = NULL;
static idx_t a2503_cnt = 0, a2503_cap = 0;
static uint64_t a2503_next = 1;

seq_t a(idx_t n) {
    while (a2503_cnt < n) {
        uint64_t k = a2503_next++;
        if (a2503_ok(k)) {
            if (a2503_cnt == a2503_cap) {
                idx_t nc = a2503_cap ? a2503_cap * 2 : 1024;
                a2503_terms = (uint64_t *)realloc(a2503_terms, nc * sizeof(uint64_t));
                a2503_cap = nc;
            }
            a2503_terms[a2503_cnt++] = k;
        }
    }
    return (seq_t)a2503_terms[n - 1];
}
