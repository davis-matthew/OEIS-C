/*
 * A117699  Palindromic primes in base 4 (written in base 4).
 * https://oeis.org/A117699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Palindromic primes in base 4 (written in base 4). The value
 *            returned is the base‑4 palindrome written as a decimal number
 *            using only digits 0‑3. For each n≥1, a(n) is the nth such
 *            palindrome
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 28 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the helper is_prime_big(seq_t) from "../utils/is_prime_big.h". */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

/* Build the decimal representation of the base‑4 palindrome and
   return the Δ prime value (decimal) via *prime_out. */
static seq_t build_pal_rep(idx_t L, idx_t prefix_val, seq_t *prime_out) {
    idx_t k = (L + 1) / 2;            /* length of left half, including middle if odd */
    idx_t digits[64];
    idx_t temp = prefix_val;
    /* extract digits of the prefix (most significant first) */
    for (idx_t i = 0; i < k; ++i) {
        digits[k-1-i] = temp % 4;
        temp /= 4;
    }

    seq_t prime_val = 0;
    seq_t rep_val   = 0;

    /* left part (including middle if odd) */
    for (idx_t i = 0; i < k; ++i) {
        prime_val = prime_val * 4 + (seq_t)digits[i];
        rep_val   = rep_val   * 10 + (seq_t)digits[i];
    }
    /* right part (mirror) */
    if (L % 2 == 0) {
        for (idx_t i = k; i > 0; --i) {
            prime_val = prime_val * 4 + (seq_t)digits[i-1];
            rep_val   = rep_val   * 10 + (seq_t)digits[i-1];
        }
    } else {
        for (idx_t i = k-1; i > 0; --i) {
            prime_val = prime_val * 4 + (seq_t)digits[i-1];
            rep_val   = rep_val   * 10 + (seq_t)digits[i-1];
        }
    }
    *prime_out = prime_val;
    return rep_val;
}

/* Pre‑compute powers of 4 up to exponent 64 (fits 64‑bit). */
static idx_t pow4_idx[65];
static bool pow4_inited = false;
static void init_pow4(void) {
    if (pow4_inited) return;
    pow4_idx[0] = 1;
    for (int i = 1; i < 65; ++i) pow4_idx[i] = pow4_idx[i-1] * 4;
    pow4_inited = true;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    static bool state_inited = false;
    static idx_t gen_len;       /* current length of palindrome being generated */
    static idx_t cur_prefix;    /* current prefix value within its range */
    static idx_t pref_start, pref_end; /* bounds for prefix */
    static idx_t cur_k;         /* k for current length */

    init_pow4();

    if (!state_inited) {
        state_inited = true;
        gen_len   = 1;
        pref_start = 1;
        pref_end   = 3;
        cur_prefix = pref_start;
        cur_k      = 1;
    }

    while (cache_len < n) {
        /* If we've exhausted the current length, move to the next length. */
        if (cur_prefix > pref_end) {
            ++gen_len;
            cur_k = (gen_len + 1) / 2;
            pref_start = pow4_idx[cur_k - 1];
            pref_end   = pow4_idx[cur_k] - 1;
            cur_prefix = pref_start;
            continue;
        }

        seq_t prime_val, rep_val;
        rep_val = build_pal_rep(gen_len, cur_prefix, &prime_val);
        ++cur_prefix; /* advance to next prefix */

        if (is_prime_big(prime_val)) {
            /* Grow cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return 0; /* allocation failure – return 0 as safe default */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = rep_val;
        }
    }

    return cache[n - 1];
}
