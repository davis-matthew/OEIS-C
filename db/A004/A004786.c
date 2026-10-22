/*
 * A004786  Numbers k such that 6!*(2*k-7)!/(k!*(k-1)!) is an integer.
 * https://oeis.org/A004786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 86, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: walk k upward from 4 and keep the prime factorisation of
   R(k) = 6!*(2k-7)!/(k!*(k-1)!) up to date, using
   R(k+1) = R(k)*(2k-5)*(2k-6)/((k+1)*k). A term is any k where no prime
   has a negative exponent in R(k). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint32_t *spf = NULL;
static int32_t *ex = NULL;
static uint64_t lim = 0;
static long neg = 0;

static void ensure(uint64_t L) {
    uint64_t newlim, i, j;
    if (L <= lim) return;
    newlim = lim * 2;
    if (newlim < L) newlim = L;
    if (newlim < 1024) newlim = 1024;
    spf = (uint32_t *)realloc(spf, (newlim + 1) * sizeof(uint32_t));
    ex = (int32_t *)realloc(ex, (newlim + 1) * sizeof(int32_t));
    if (lim == 0) memset(ex, 0, (newlim + 1) * sizeof(int32_t));
    else memset(ex + lim + 1, 0, (newlim - lim) * sizeof(int32_t));
    memset(spf, 0, (newlim + 1) * sizeof(uint32_t));
    for (i = 2; i <= newlim; i++) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            for (j = i * i; j <= newlim; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    lim = newlim;
}

static void addf(uint64_t x, int sign) {
    while (x > 1) {
        uint64_t p = spf[x];
        while (x % p == 0) {
            int32_t old = ex[p];
            int32_t nw = old + sign;
            x /= p;
            ex[p] = nw;
            if (old < 0 && nw >= 0) neg--;
            else if (old >= 0 && nw < 0) neg++;
        }
    }
}

seq_t a(idx_t n) {
    static uint64_t *cache = NULL;
    static idx_t cnt = 0, cap = 0;
    static uint64_t curk = 0;
    static bool init = false;

    if (!init) {
        init = true;
        ensure(64);
        addf(720, 1);
        addf(4, -1); addf(3, -1); addf(2, -1);
        addf(3, -1); addf(2, -1);
        curk = 4;
        if (neg == 0) {
            cap = 64;
            cache = (uint64_t *)malloc(cap * sizeof(uint64_t));
            cache[cnt++] = curk;
        }
    }
    while (cnt < n) {
        uint64_t k = curk;
        ensure(2 * k + 2);
        addf(2 * k - 5, 1);
        addf(2 * k - 6, 1);
        addf(k + 1, -1);
        addf(k, -1);
        curk = k + 1;
        if (neg == 0) {
            if (cnt >= cap) {
                cap = cap ? cap * 2 : 64;
                cache = (uint64_t *)realloc(cache, cap * sizeof(uint64_t));
            }
            cache[cnt++] = curk;
        }
    }
    return (seq_t)cache[n - 1];
}
