/*
 * A234746  Self-inverse permutation of integers induced by the restriction of
 *          blue-code to reducible polynomials over GF(2): a(n) =
 *          A091246(A193231(A091242(n))).
 * https://oeis.org/A234746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A091246(A193231(A091242(n))); computed by search: n-th
 *            reducible GF(2)[x] polynomial, substitute x+1, then rank among
 *            reducibles
 * Range:     u8 n <= 243, u16 n <= 15005, u32 n <= 15005, u64 n <= 15005,
 *            u128 n <= 15005, big any n
 * Validated: standard, pass, 92 terms through n = 9950 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static unsigned A234746_bits(uint64_t x)
{
    unsigned r = 0;
    while (x) { ++r; x >>= 1; }
    return r;
}

static uint64_t A234746_pmul(uint64_t a, uint64_t b)
{
    uint64_t r = 0;
    while (b) {
        if (b & 1u) r ^= a;
        a <<= 1;
        b >>= 1;
    }
    return r;
}

static uint64_t A234746_pmod(uint64_t a, uint64_t mod)
{
    unsigned dmod = A234746_bits(mod);
    while (a && A234746_bits(a) >= dmod) {
        unsigned sh = A234746_bits(a) - dmod;
        a ^= (mod << sh);
    }
    return a;
}

static uint64_t A234746_pgcd(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t r = A234746_pmod(a, b);
        a = b;
        b = r;
    }
    return a;
}

static bool A234746_is_reducible(uint64_t f)
{
    if (f <= 3u) return false;
    unsigned d = A234746_bits(f) - 1;
    uint64_t xpow = 2u;
    for (unsigned i = 1; i <= d / 2; ++i) {
        xpow = A234746_pmod(A234746_pmul(xpow, xpow), f);
        uint64_t g = A234746_pgcd(f, xpow ^ 2u);
        if (g != 1u) return true;
    }
    return false;
}

static uint64_t A234746_blue(uint64_t n)
{
    uint64_t res = 0, mask = 1;
    while (n) {
        if (n & 1u) res ^= mask;
        mask = (mask << 1) ^ mask;
        n >>= 1;
    }
    return res;
}

static uint64_t *A234746_list = NULL;
static size_t A234746_len = 0, A234746_cap = 0;
static uint64_t A234746_last = 0;

static void A234746_step(void)
{
    ++A234746_last;
    if (A234746_is_reducible(A234746_last)) {
        if (A234746_len == A234746_cap) {
            size_t nc = A234746_cap ? A234746_cap * 2 : 64;
            uint64_t *p = (uint64_t *)realloc(A234746_list, nc * sizeof(uint64_t));
            if (!p) exit(1);
            A234746_list = p;
            A234746_cap = nc;
        }
        A234746_list[A234746_len++] = A234746_last;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (A234746_len < (size_t)n) A234746_step();
    uint64_t k = A234746_list[n - 1];
    uint64_t m = A234746_blue(k);
    while (A234746_last < m) A234746_step();
    /* count reducibles <= m via upper bound */
    size_t lo = 0, hi = A234746_len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (A234746_list[mid] <= m) lo = mid + 1;
        else hi = mid;
    }
    return (seq_t)lo;
}
