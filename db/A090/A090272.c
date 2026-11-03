/*
 * A090272  Take each palindrome ending in 1, 3, 7, or 9 and find smallest
 *          prime formed by the digits of that palindrome, followed by a
 *          string of digits, followed by the palindrome again.
 * https://oeis.org/A090272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   computes A090272 – the smallest prime of the form P S P where P
 *            is the n‑th palindrome ending in 1, 3, 7 or 9 and S is any
 *            (possibly empty) string of digits. This implementation builds
 *            the palindromes in increasing order, searches for the first
 *            prime with minimal numeric value, and caches results...
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits_idx.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
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

/* Map a 0‑based index (0,1,2,3) to the first digit of a palindrome. */
static inline uint64_t first_digit_from_index(int idx) {
    static const uint64_t num_digits_idx[4] = {1, 3, 7, 9};
    return num_digits_idx[idx];
}

/* Reverse an unsigned 64‑bit integer (used to construct palindromes). */
static inline uint64_t reverse_uint64(uint64_t x) {
    uint64_t rev = 0;
    while (x > 0) {
        rev = rev * 10 + (x % 10);
        x /= 10;
    }
    return rev;
}

/* Compute 10ⁿ as a 64‑bit integer (small exponents only). */
static inline uint64_t pow10_u64(int n) {
    uint64_t v = 1;
    while (n-- > 0) v *= 10;
    return v;
}

/* Return the n‑th palindrome (1‑indexed) that ends in 1, 3, 7 or 9. */
static uint64_t nth_palindrome(idx_t n) {
    idx_t idx = n - 1;          /* 0‑based index */
    int L = 1;
    while (1) {
        int half;
        size_t countLen;
        if (L % 2 == 0) {              /* even length */
            half = L / 2;
        } else {                       /* odd length */
            half = (L + 1) / 2;
        }
        countLen = 4ULL * pow10_u64(half - 1);   /* 4 * 10^(half‑1) */

        if ((idx_t)idx < (idx_t)countLen) {
            /* Inside this length L. */
            int firstIdx = idx / (int)pow10_u64(half - 1);   /* 0‑3 */
            int rem      = idx % (int)pow10_u64(half - 1);   /* 0‑10^(half‑1)-1 */
            uint64_t firstDigit = first_digit_from_index(firstIdx);
            uint64_t prefix = firstDigit * pow10_u64(half - 1) + (uint64_t)rem;

            uint64_t pal;
            if (L % 2 == 0) {   /* even length */
                pal = prefix * pow10_u64(half) + reverse_uint64(prefix);
            } else {            /* odd length */
                pal = prefix * pow10_u64(half - 1) + reverse_uint64(prefix / 10);
            }
            return pal;
        }

        idx -= countLen;
        ++L;
    }
}

/* Find the smallest prime of the form P S P for a given palindrome P. */
static seq_t smallest_prime_for_p(uint64_t p) {
    int lenP = num_digits_idx((idx_t)p);
    seq_t powLenP = pow10((idx_t)lenP);

    for (int lenS = 0; ; ++lenS) {
        seq_t powLenS      = pow10((idx_t)lenS);               /* 10^lenS */
        seq_t powLenSP     = powLenS * powLenP;                 /* 10^(lenS+lenP) */
        seq_t base         = ((seq_t)p) * powLenSP + ((seq_t)p);/* p·10^(lenS+lenP)+p */

        size_t maxS = (size_t)powLenS;   /* 10^lenS fits in size_t for small lenS */
        for (size_t s = 0; s < maxS; ++s) {
            seq_t N = base + ((seq_t)s) * powLenP;
            if (is_prime_big(N)) return N;
        }
    }
}

/* External linkage as required. */
seq_t a(idx_t n) {
    /* Static cache of terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    if ((size_t)n > cache_len) {
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
        cache = new_cache;
        cache_cap = new_cap;
    }

    for (size_t i = cache_len; i < (size_t)n; ++i) {
        uint64_t p = nth_palindrome((idx_t)(i + 1));  /* 1‑indexed index */
        cache[i] = smallest_prime_for_p(p);
    }
    cache_len = (size_t)n;

    return cache[(size_t)n - 1];
}
