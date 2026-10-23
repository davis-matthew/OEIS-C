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
 * Helpers:   build_pal_rep, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the helper is_prime_big(seq_t) from "../utils/is_prime_big.h". */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/build_pal_rep.h"

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
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0; /* allocation failure – return 0 as safe default */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = rep_val;
        }
    }

    return cache[n - 1];
}
