/*
 * A007419  Largest number not the sum of distinct n-th-order polygonal
 *          numbers.
 * https://oeis.org/A007419
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is found by a subset-sum sieve. Polygonals P(n,k) = k +
 *            (n-2)*k*(k-1)/2 are added one at a time to a bitset of reachable
 *            sums. Stop once the largest unreachable value u below S/2 is
 *            followed by a contiguous run of reachable values that every
 *            later polygonal extends
 * Range:     u8 n <= 5, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 44 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint64_t poly_k(uint64_t n, uint64_t k) {
    return k + k * (k - 1) / 2 * (n - 2);
}

static void shift_or(uint64_t *w, uint64_t nw, uint64_t p) {
    uint64_t q = p >> 6, r = p & 63, i;
    if (q >= nw) return;
    if (r == 0) {
        for (i = nw - 1; i >= q; i--) {
            w[i] |= w[i - q];
            if (i == q) break;
        }
    } else {
        for (i = nw - 1; i > q; i--)
            w[i] |= (w[i - q] << r) | (w[i - q - 1] >> (64 - r));
        w[q] |= w[0] << r;
    }
}

/* largest unset bit index in [0, L], or -1 if none */
static long long top_unset(const uint64_t *w, uint64_t L) {
    uint64_t nw = (L >> 6) + 1;
    uint64_t i = nw;
    while (i > 0) {
        uint64_t mask, inv;
        i--;
        if (i == nw - 1) {
            uint64_t bits = (L & 63) + 1;
            mask = bits == 64 ? ~(uint64_t)0 : (((uint64_t)1 << bits) - 1);
        } else {
            mask = ~(uint64_t)0;
        }
        inv = ~w[i] & mask;
        if (inv) return (long long)(i * 64 + 63 - (uint64_t)__builtin_clzll(inv));
    }
    return -1;
}

seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    uint64_t cap = 1024, S = 0, k = 1, res = 0;
    uint64_t *w = (uint64_t *)calloc((size_t)cap, sizeof(uint64_t));
    w[0] = 1;
    for (;;) {
        uint64_t p = poly_k(nn, k), nw;
        S += p;
        nw = (S >> 6) + 1;
        if (nw > cap) {
            uint64_t nc = cap;
            while (nc < nw) nc *= 2;
            w = (uint64_t *)realloc(w, (size_t)nc * sizeof(uint64_t));
            memset(w + cap, 0, (size_t)(nc - cap) * sizeof(uint64_t));
            cap = nc;
        }
        shift_or(w, nw, p);
        k++;
        if (k >= 3) {
            long long u = top_unset(w, S / 2);
            if (u >= 0) {
                uint64_t uu = (uint64_t)u;
                uint64_t p1 = poly_k(nn, k), p2 = poly_k(nn, k + 1);
                if (S >= 2 * uu + 1) {
                    uint64_t run = S - 2 * uu - 1;
                    if (uu < p1 && p1 <= run && p2 <= run + p1) {
                        res = uu;
                        break;
                    }
                }
            }
        }
    }
    free(w);
    return (seq_t)res;
}
