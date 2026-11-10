/*
 * A234853  Indices of primes in A234851.
 * https://oeis.org/A234853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m such that A234851(m) is prime; A234851(k) = k-th
 *            index j with prime(j)-j+1 prime. Computed by search
 * Range:     u8 n <= 11, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-07)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

static bool A234851_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool A234851_is_prime_big(seq_t n) {
    if (n < 2) return false;
    if (A234851_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

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
        if (x == 1 || x == n-1) continue;
        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t *A234851_cache = NULL;
static size_t A234851_cache_len = 0;
static size_t A234851_cache_cap = 0;

static seq_t *A234851_primes = NULL;
static size_t A234851_primes_len = 0;
static size_t A234851_primes_cap = 0;

static idx_t A234851_next_k = 1;
static idx_t A234851_next_candidate = 2;

static void A234851_grow(seq_t **arr, size_t *cap, size_t new_len)
{
    if (new_len > *cap) {
        size_t new_cap = (*cap == 0) ? 1 : (*cap << 1);
        while (new_cap < new_len) new_cap <<= 1;
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
}

static seq_t A234851_a(idx_t n)
{
    size_t target = (size_t)n;
    while (A234851_cache_len < target) {
        while (A234851_primes_len < (size_t)A234851_next_k) {
            while (!is_prime(A234851_next_candidate))
                ++A234851_next_candidate;
            A234851_grow(&A234851_primes, &A234851_primes_cap, A234851_primes_len + 1);
            A234851_primes[A234851_primes_len++] = (seq_t)A234851_next_candidate;
            ++A234851_next_candidate;
        }
        seq_t pk = A234851_primes[(size_t)A234851_next_k - 1];
        seq_t val = pk - (seq_t)A234851_next_k + 1;
        if (A234851_is_prime_big(val)) {
            A234851_grow(&A234851_cache, &A234851_cache_cap, A234851_cache_len + 1);
            A234851_cache[A234851_cache_len++] = (seq_t)A234851_next_k;
        }
        ++A234851_next_k;
    }
    return A234851_cache[target - 1];
}

static seq_t *res_cache = NULL;
static size_t res_len = 0;
static size_t res_cap = 0;
static idx_t res_next_m = 1;

seq_t a(idx_t n)
{
    size_t target = (size_t)n;
    while (res_len < target) {
        seq_t v = A234851_a(res_next_m);
        if (A234851_is_prime_big(v)) {
            A234851_grow(&res_cache, &res_cap, res_len + 1);
            res_cache[res_len++] = (seq_t)res_next_m;
        }
        ++res_next_m;
    }
    return res_cache[target - 1];
}
