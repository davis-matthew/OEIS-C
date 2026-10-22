/*
 * A003964  Fully multiplicative with a(prime(k)) = partition(k+1).
 * https://oeis.org/A003964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, mult
 * Formula:   a(n) = Product partition(pi(p)+1)^e over p^e || n, partitions by
 *            Euler's pentagonal recurrence
 * Range:     u8 n <= 52, u16 n <= 190, u32 n <= 708, u64 n <= 2860,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_zero.h"

static uint32_t *spf = NULL;   /* smallest prime factor */
static uint32_t *pic = NULL;   /* prime counting function */
static uint64_t slim = 0;

static void ensure_sieve(uint64_t n) {
    uint64_t L, i, j;
    if (n <= slim) return;
    L = (n < 2 * slim) ? 2 * slim : n;
    if (L < 64) L = 64;
    free(spf);
    free(pic);
    spf = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    pic = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
        pic[i] = pic[i - 1] + (spf[i] == i ? 1u : 0u);
    }
    slim = L;
}

static seq_t *P = NULL;
static idx_t Plen = 0;

static seq_t partition(idx_t m) {
    if (m >= Plen) {
        idx_t newlen = (m + 1 < 2 * Plen) ? 2 * Plen : m + 1;
        idx_t i;
        P = grow_zero(P, Plen, newlen);
        for (i = Plen; i < newlen; i++) {
            if (i == 0) { P[0] = 1; continue; }
            seq_t pos = 0, neg = 0;
            idx_t k;
            for (k = 1; ; k++) {
                idx_t g1 = k * (3 * k - 1) / 2;
                idx_t g2 = k * (3 * k + 1) / 2;
                if (g1 > i) break;
                if (k & 1) {
                    pos += P[i - g1];
                    if (g2 <= i) pos += P[i - g2];
                } else {
                    neg += P[i - g1];
                    if (g2 <= i) neg += P[i - g2];
                }
            }
            P[i] = pos - neg;
        }
        Plen = newlen;
    }
    return P[m];
}

seq_t a(idx_t n) {
    seq_t result = 1;
    uint64_t x = n;
    if (n <= 1) return 1;
    ensure_sieve(n);
    while (x > 1) {
        uint64_t p = spf[x];
        seq_t f = partition((idx_t)pic[p] + 1);
        while (x % p == 0) {
            x /= p;
            result *= f;
        }
    }
    return result;
}
