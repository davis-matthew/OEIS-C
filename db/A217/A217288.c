/*
 * A217288  Records in A217287.
 * https://oeis.org/A217288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. The chain starting at s stops at the
 *            first m > s whose prime factors all divide some number in [s,
 *            m-1]. A prime p | m divides a number in that range iff m - p >=
 *            s, so the chain length is A217287(s) = min{ m > s : m - gpf(m)
 *            >= s } - s, where gpf is the greatest prime...
 * Range:     u8 n <= 43, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define A217288_SEG (1u << 18)
#define A217288_PLIM 65536u
#define A217288_MAXREC 4096

static uint32_t A217288_primes[7000];
static size_t A217288_np = 0;
static uint32_t A217288_rem[A217288_SEG];
static uint32_t A217288_gp[A217288_SEG];

static void A217288_init_primes(void)
{
    static unsigned char comp[A217288_PLIM + 1];
    for (uint32_t i = 2; i <= A217288_PLIM; ++i) {
        if (!comp[i]) {
            A217288_primes[A217288_np++] = i;
            for (uint64_t j = (uint64_t)i * i; j <= A217288_PLIM; j += i)
                comp[j] = 1;
        }
    }
}

/* fills greatest prime factor for m in [lo, lo+len) */
static void A217288_fill(uint64_t lo, uint32_t len)
{
    uint64_t hi = lo + len;
    for (uint32_t i = 0; i < len; ++i) {
        A217288_rem[i] = (uint32_t)(lo + i);
        A217288_gp[i] = 1;
    }
    for (size_t k = 0; k < A217288_np; ++k) {
        uint32_t p = A217288_primes[k];
        if ((uint64_t)p * p >= hi) break;
        uint64_t start = ((lo + p - 1) / p) * p;
        for (uint64_t j = start - lo; j < len; j += p) {
            uint32_t r = A217288_rem[j];
            do { r /= p; } while (r % p == 0);
            A217288_rem[j] = r;
            A217288_gp[j] = p;
        }
    }
    for (uint32_t i = 0; i < len; ++i)
        if (A217288_rem[i] > 1) A217288_gp[i] = A217288_rem[i];
}

seq_t a(idx_t n)
{
    static uint64_t rec[A217288_MAXREC];
    static size_t cnt = 0;
    static uint64_t best = 0;
    static uint64_t s = 1;          /* smallest start whose chain is unresolved */
    static uint64_t lo = 2;         /* current segment start */
    static uint32_t len = 0, pos = 0;
    static bool inited = false;

    if (n < 1) return 0;
    if (!inited) { A217288_init_primes(); inited = true; }

    while (cnt < (size_t)n && cnt < A217288_MAXREC) {
        if (pos == len) {
            uint64_t nlo = lo + len;
            if (nlo >= 0xFFFFFFFFull) break;
            uint64_t l = A217288_SEG;
            if (nlo + l > 0xFFFFFFFFull) l = 0xFFFFFFFFull - nlo;
            lo = nlo;
            len = (uint32_t)l;
            pos = 0;
            A217288_fill(lo, len);
        }
        uint64_t m = lo + pos;
        uint64_t g = m - A217288_gp[pos];
        while (g >= s) {
            uint64_t L = m - s;
            if (L > best) {
                best = L;
                if (cnt < A217288_MAXREC) rec[cnt++] = L;
            }
            ++s;
        }
        ++pos;
    }
    if ((size_t)n > cnt) return 0;
    return (seq_t)rec[n - 1];
}
