/*
 * A004784  Numbers k such that 4!*(2k-5)!/(k!*(k-1)!) is an integer.
 * https://oeis.org/A004784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over k >= 3. Since 4!*(2k-5)!/(k!*(k-1)!) =
 *            24*C(k-1)/((2k-2)(2k-3)(2k-4)), with C the Catalan numbers, only
 *            primes dividing 2*(k-1)*(k-2)*(2k-3) (and 3) can fail. For each
 *            such p, check with Legendre's formula that v_p(24) +
 *            v_p((2k-5)!) - v_p(k!) - v_p((k-1)!) >= 0. Prime factors come
 *            fr...
 * Range:     u8 n <= 4, u16 n <= 724, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static uint32_t *spf = NULL;
static uint64_t spf_lim = 0;
static uint64_t *terms = NULL;
static uint64_t tcount = 0, tcap = 0;
static uint64_t next_k = 3;

static void ensure_spf(uint64_t lim) {
    uint64_t newlim, i, j;
    if (lim <= spf_lim) return;
    newlim = spf_lim * 2;
    if (newlim < lim) newlim = lim;
    if (newlim < 65536) newlim = 65536;
    free(spf);
    spf = (uint32_t *)calloc(newlim + 1, sizeof(uint32_t));
    for (i = 2; i <= newlim; i++) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            for (j = i * i; j <= newlim; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    spf_lim = newlim;
}

static uint64_t vfact(uint64_t n, uint64_t p) {
    uint64_t r = 0;
    while (n) { n /= p; r += n; }
    return r;
}

static bool prime_ok(uint64_t k, uint64_t p) {
    uint64_t num = vfact(2 * k - 5, p);
    if (p == 2) num += 3;
    else if (p == 3) num += 1;
    return num >= vfact(k, p) + vfact(k - 1, p);
}

static bool check_number(uint64_t k, uint64_t x) {
    while (x > 1) {
        uint64_t p = spf[x];
        if (!prime_ok(k, p)) return false;
        while (x % p == 0) x /= p;
    }
    return true;
}

static bool is_valid(uint64_t k) {
    if (!prime_ok(k, 2) || !prime_ok(k, 3)) return false;
    if (!check_number(k, k - 1)) return false;
    if (!check_number(k, k - 2)) return false;
    if (!check_number(k, 2 * k - 3)) return false;
    return true;
}

seq_t a(idx_t n) {
    while (tcount < (uint64_t)n) {
        uint64_t k = next_k++;
        ensure_spf(2 * k);
        if (is_valid(k)) {
            if (tcount == tcap) {
                tcap = tcap ? tcap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, tcap * sizeof(uint64_t));
            }
            terms[tcount++] = k;
        }
    }
    return (seq_t)terms[n - 1];
}
