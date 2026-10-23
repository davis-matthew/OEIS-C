/*
 * A137091  Numbers k such that k and k^2 use only the digits 2, 3, 6 and 9.
 * https://oeis.org/A137091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A137091: numbers k such that k and k^2 use only the digits 2, 3, 6 and 9.
   This implementation enumerates candidates in increasing order and caches
   the results for efficient repeated look‑ups. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- helpers ---------- */

/* Product of two seq_t values.  For small unsigned types this uses
   unsigned __int128 to avoid overflow; for arbitrary‑precision types
   the class multiplication operator is used. */
#ifdef __cplusplus
#include <type_traits>
template <typename T>
inline seq_t sqr(T x) {
    if constexpr (std::is_same_v<T, unsigned long long>) {
        __uint128_t t = static_cast<__uint128_t>(x) * x;
        return (seq_t)t;
    } else {
        return x * x;
    }
}
#else
static inline seq_t sqr(seq_t x) {
    __uint128_t t = (__uint128_t)x * x;
    return (seq_t)t;
}
#endif

/* Return true if all decimal digits of x are in {2,3,6,9}. */
static bool good_digits(seq_t x) {
    while (x > 0) {
        uint8_t d = (uint8_t)(x % 10);
        if (d != 2 && d != 3 && d != 6 && d != 9)
            return false;
        x /= 10;
    }
    return true;
}

/* ---------- cache and state ---------- */

static seq_t *cache      = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static int    next_len   = 1;           /* current number of digits */
static __uint128_t next_idx = 0;        /* base‑4 index at current length */
static seq_t  seq_t_max;               /* maximum value of seq_t for small types */
static bool   seq_t_max_inited = false;

/* Pre‑computed powers of 4 up to 4^64. */
static __uint128_t pow4[65];
static bool pow4_inited = false;

static void init_pow4(void) {
    if (pow4_inited) return;
    pow4[0] = 1;
    for (int i = 1; i < 65; ++i) pow4[i] = pow4[i - 1] * 4ULL;
    pow4_inited = true;
}

static void init_seq_t_max(void) {
    if (seq_t_max_inited) return;
    if (sizeof(seq_t) <= 8) {
        seq_t_max = (seq_t)-1;
    }
    seq_t_max_inited = true;
}

static void ensure_cache_capacity(void) {
    if (cache_cap == 0) {
        cache = (seq_t *)malloc(16 * sizeof(seq_t));
        if (!cache) abort();
        cache_cap = 16;
    } else if (cache_size >= cache_cap) {
        cache_cap *= 2;
        seq_t *new_mem = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!new_mem) abort();
        cache = new_mem;
    }
}

/* ---------- generation ---------- */

static void generate_terms_until(idx_t n) {
    init_pow4();
    init_seq_t_max();

    const uint8_t digits[4] = {2, 3, 6, 9};

    while ((idx_t)cache_size < n) {
        /* when all numbers of the current length are exhausted, move to next length */
        if (next_idx >= pow4[next_len]) {
            ++next_len;
            next_idx = 0;
            continue;
        }

        /* build the candidate from the base‑4 index */
        seq_t cand = 0;
        bool overflowed = false;
        __uint128_t idx = next_idx;

        /* extract digits from least sig. to most sig. */
        uint8_t temp_digits[128];  /* enough for very large lengths */
        int pos = 0;
        while (pos < next_len) {
            temp_digits[pos] = digits[(uint8_t)(idx & 3ULL)];
            idx >>= 2;
            ++pos;
        }

        /* assemble the candidate in correct order (most sig. first) */
        for (int i = next_len - 1; i >= 0; --i) {
            cand = cand * 10 + temp_digits[i];
            if (sizeof(seq_t) <= 8 && cand > seq_t_max) {
                overflowed = true;
                break;
            }
        }

        if (!overflowed) {
            seq_t sq = sqr(cand);
            if (good_digits(sq)) {
                ensure_cache_capacity();
                cache[cache_size++] = cand;
            }
        }

        ++next_idx;
    }
}

/* ---------- public function ---------- */

seq_t a(idx_t n) {
    if (n == 0)                /* offset is 1; undefined for n<1 */
        return 0;
    generate_terms_until(n);
    return cache[n - 1];
}
