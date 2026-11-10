/*
 * A084721  Primes arising in A083569.
 * https://oeis.org/A084721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + A083569(n), where A083569(n) is the smallest m not
 *            occurring earlier with m+n prime (greedy search with sieve-free
 *            primality tests)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-07)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

static bool A084721_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool A084721_is_prime_big(seq_t n) {
    if (n < 2) return false;
    if (A084721_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);
    for (int i = 0; i < num_bases; i++) {
        seq_t b = (seq_t)base_list[i];
        if (b >= n) continue;
        seq_t x = 1;
        seq_t pw = b % n;
        seq_t e = d;
        while (e > 0) {
            if (e & 1) x = (x * pw) % n;
            pw = (pw * pw) % n;
            e >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t *A084721_cache = NULL;
static size_t A084721_cache_cap = 0;
static idx_t A084721_cache_len = 0;
static bool *A084721_used = NULL;
static size_t A084721_used_cap = 0;

static void A084721_ensure_used(size_t need)
{
    if (need <= A084721_used_cap) return;
    size_t new_cap = A084721_used_cap ? A084721_used_cap << 1 : 1;
    while (new_cap < need) new_cap <<= 1;
    bool *tmp = (bool *)realloc(A084721_used, new_cap * sizeof(bool));
    if (!tmp) return;
    memset(tmp + A084721_used_cap, 0, (new_cap - A084721_used_cap) * sizeof(bool));
    A084721_used = tmp;
    A084721_used_cap = new_cap;
}

static void A084721_ensure_cache(idx_t need)
{
    if ((size_t)need <= A084721_cache_cap) return;
    size_t new_cap = A084721_cache_cap ? A084721_cache_cap << 1 : 1;
    while (new_cap < (size_t)need) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, A084721_cache, new_cap);
    if (!tmp) return;
    A084721_cache = tmp;
    A084721_cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    A084721_ensure_cache(n);
    for (idx_t k = A084721_cache_len + 1; k <= n; ++k) {
        seq_t m = 1;
        for (;;) {
            if (m > (seq_t)SIZE_MAX) break;
            size_t idx_m = (size_t)m;
            A084721_ensure_used(idx_m + 1);
            if (!A084721_used[idx_m]) {
                if (A084721_is_prime_big(m + (seq_t)k)) break;
            }
            ++m;
        }
        A084721_cache[(size_t)(k - 1)] = m;
        A084721_used[(size_t)m] = true;
        ++A084721_cache_len;
    }
    return (seq_t)n + A084721_cache[(size_t)(n - 1)];
}
