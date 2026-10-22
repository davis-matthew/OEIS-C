/*
 * A003650  Class number of real quadratic field with discriminant 4n+1.
 * https://oeis.org/A003650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = h(D), the class number of Q(sqrt(D)), where D is the n-th
 *            squarefree D == 1 (mod 4) with D > 1. The code finds h by
 *            search: it lists the reduced forms of discriminant D, counts the
 *            rho-cycles to get the narrow class number h+, and halves h+ when
 *            the fundamental unit has norm +1
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   isqrt64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/isqrt64.h"

static uint64_t *dlist = 0;
static size_t dlen = 0, dcap = 0;
static uint64_t dnext = 1;

static bool sqfree_odd(uint64_t m) {
    for (uint64_t p = 3; p * p <= m; p += 2)
        if (m % (p * p) == 0) return false;
    return true;
}

static uint64_t nth_disc(idx_t n) {
    while (dlen < n) {
        dnext += 4;
        if (sqfree_odd(dnext)) {
            if (dlen == dcap) {
                dcap = dcap ? 2 * dcap : 1024;
                dlist = (uint64_t *)realloc(dlist, dcap * sizeof(uint64_t));
            }
            dlist[dlen++] = dnext;
        }
    }
    return dlist[n - 1];
}

static int64_t pmod(int64_t x, int64_t m) {
    int64_t r = x % m;
    return r < 0 ? r + m : r;
}

static int64_t classno(int64_t D) {
    int64_t s = isqrt64(D);
    size_t cnt = 0, cap = 256;
    int64_t *FA = (int64_t *)malloc(cap * sizeof(int64_t));
    int64_t *FB = (int64_t *)malloc(cap * sizeof(int64_t));
    int64_t *FC = (int64_t *)malloc(cap * sizeof(int64_t));
    for (int64_t b = 1; b <= s; b += 2) {
        int64_t m = (D - b * b) / 4;
        for (int64_t d = 1; 2 * d - b <= s; d++) {
            if (2 * d + b <= s) continue;
            if (m % d != 0) continue;
            if (cnt + 2 > cap) {
                cap *= 2;
                FA = (int64_t *)realloc(FA, cap * sizeof(int64_t));
                FB = (int64_t *)realloc(FB, cap * sizeof(int64_t));
                FC = (int64_t *)realloc(FC, cap * sizeof(int64_t));
            }
            FA[cnt] = d;  FB[cnt] = b; FC[cnt] = -(m / d); cnt++;
            FA[cnt] = -d; FB[cnt] = b; FC[cnt] = m / d;    cnt++;
        }
    }
    bool *seen = (bool *)calloc(cnt ? cnt : 1, sizeof(bool));
    int64_t cycles = 0;
    bool normm1 = false;
    for (size_t i = 0; i < cnt; i++) {
        if (seen[i]) continue;
        cycles++;
        bool has1 = false, hasm1 = false;
        size_t j = i;
        while (!seen[j]) {
            seen[j] = true;
            if (FA[j] == 1) has1 = true;
            if (FA[j] == -1) hasm1 = true;
            int64_t na = FC[j];
            int64_t md = 2 * (na < 0 ? -na : na);
            int64_t r = pmod(-FB[j], md);
            int64_t nb = s - pmod(s - r, md);
            int64_t nc = (nb * nb - D) / (4 * na);
            size_t k;
            for (k = 0; k < cnt; k++)
                if (FA[k] == na && FB[k] == nb && FC[k] == nc) break;
            if (k == cnt) break;
            j = k;
        }
        if (has1 && hasm1) normm1 = true;
    }
    free(FA); free(FB); free(FC); free(seen);
    return normm1 ? cycles : cycles / 2;
}

seq_t a(idx_t n) {
    int64_t D = (int64_t)nth_disc(n);
    return (seq_t)(uint64_t)classno(D);
}
