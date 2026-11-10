/*
 * A090267  Primes arising in A090266.
 * https://oeis.org/A090267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = concat(k, prime(n), k), where k = A090266(n) is the least
 *            k >= 1 that makes this concatenation prime (found by searching k
 *            = 1, 2, ...)
 * Range:     u8 none, u16 n <= 4, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-07)
 * Helpers:   is_prime_u64, mulmod_seq, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/num_digits_idx.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod_seq.h"

static uint32_t A090266_get_prime(idx_t k)
{
    static uint32_t *pr = NULL;
    static size_t sz   = 0;
    static size_t cap  = 0;
    static uint32_t cand = 3;

    if (cap == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
    }
    while (sz < (size_t)k) {
        if (sz == 0) { pr[sz++] = 2; continue; }
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }
        if (!ok) { cand += 2; continue; }
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }
    return pr[(size_t)k - 1];
}

static bool A090266_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool A090266_is_prime_big(seq_t n)
{
    if (n < 2) return false;
    if (A090266_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if ((n & 1) == 0) return false;

    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }

    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = (int)(sizeof(base_list) / sizeof(base_list[0]));

    for (int i = 0; i < num_bases; i++) {
        seq_t b = (seq_t)base_list[i];
        if (b >= n) continue;
        seq_t x = 1;
        seq_t pw = b % n;
        seq_t ex = d;
        while (ex > 0) {
            if (ex & 1) x = mulmod_seq(x, pw, n);
            pw = mulmod_seq(pw, pw, n);
            ex >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = mulmod_seq(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t A090266_pow10(idx_t e)
{
    seq_t res = 1;
    while (e) { res *= 10; e--; }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint32_t p32 = A090266_get_prime(n);
    seq_t p = (seq_t)p32;
    seq_t pow10p = A090266_pow10((idx_t)num_digits_idx((idx_t)p32));

    for (idx_t k = 1;; ++k) {
        seq_t kseq = (seq_t)k;
        seq_t pow10k = A090266_pow10((idx_t)num_digits_idx(k));
        seq_t cand = (kseq * pow10p + p) * pow10k + kseq;
        if (A090266_is_prime_big(cand)) return cand;
    }
}
