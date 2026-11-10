/*
 * A083208  a(n) = A083206(A083207(n)).
 * https://oeis.org/A083208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A083206(A083207(n)); the n-th Zumkeller number z is found
 *            by search (bitset subset-sum), then a(n) = number of subsets of
 *            the divisors of z other than z that sum to sigma(z)/2 - z
 * Range:     u8 n <= 36, u16 n <= 161, u32 n <= 567, u64 n <= 3470,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9226 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

static size_t divisors_of(uint64_t n, uint64_t *divs, uint64_t *sig)
{
    size_t c = 0;
    uint64_t s = 0;
    for (uint64_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divs[c++] = i; s += i;
            if (i != n / i) { divs[c++] = n / i; s += n / i; }
        }
    }
    *sig = s;
    return c;
}

static bool A083207_is_zumkeller(uint64_t n)
{
    uint64_t divs[4096];
    uint64_t sg;
    size_t dc = divisors_of(n, divs, &sg);
    if (sg & 1) return false;
    if (sg < 2 * n) return false;
    uint64_t t = (sg - 2 * n) / 2;
    if (t == 0) return true;
    size_t words = (size_t)(t >> 6) + 1;
    uint64_t *bits = (uint64_t *)calloc(words, sizeof(uint64_t));
    if (!bits) abort();
    bits[0] = 1ULL;
    for (size_t i = 0; i < dc; ++i) {
        uint64_t d = divs[i];
        if (d == n || d > t) continue;
        size_t ws = (size_t)(d >> 6);
        unsigned bs = (unsigned)(d & 63);
        for (size_t j = words; j-- > ws; ) {
            uint64_t v = bits[j - ws];
            if (!v) continue;
            if (bs == 0) bits[j] |= v;
            else {
                bits[j] |= v << bs;
                if (j + 1 < words) bits[j + 1] |= v >> (64 - bs);
            }
        }
        if ((bits[t >> 6] >> (t & 63)) & 1ULL) break;
    }
    bool ok = ((bits[t >> 6] >> (t & 63)) & 1ULL) != 0;
    free(bits);
    return ok;
}

static uint64_t *zk_list = NULL;
static size_t zk_len = 0, zk_cap = 0;
static uint64_t zk_last = 5;

static uint64_t nth_zumkeller(idx_t n)
{
    while (zk_len < (size_t)n) {
        ++zk_last;
        if (A083207_is_zumkeller(zk_last)) {
            if (zk_len == zk_cap) {
                zk_cap = zk_cap ? zk_cap * 2 : 256;
                zk_list = (uint64_t *)realloc(zk_list, zk_cap * sizeof(uint64_t));
                if (!zk_list) abort();
            }
            zk_list[zk_len++] = zk_last;
        }
    }
    return zk_list[n - 1];
}

seq_t a(idx_t n)
{
    uint64_t z = nth_zumkeller(n);
    uint64_t divs[4096];
    uint64_t sg;
    size_t dc = divisors_of(z, divs, &sg);
    uint64_t t = (sg - 2 * z) / 2;
    seq_t *dp = mem_new(seq_t, (size_t)t + 1);
    dp[0] = 1;
    for (size_t k = 0; k < dc; ++k) {
        uint64_t d = divs[k];
        if (d == z || d > t) continue;
        for (size_t s = (size_t)t; s >= (size_t)d; --s)
            dp[s] = dp[s] + dp[s - (size_t)d];
    }
    seq_t r = dp[t];
    mem_free(dp);
    return r;
}
