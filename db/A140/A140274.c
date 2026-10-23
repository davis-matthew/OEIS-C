/*
 * A140274  a(1)=1; a(n)=least nonnegative integer such that the concatenation
 *          of all terms, including a(n), is == 1 (mod n).
 * https://oeis.org/A140274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 177, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   digit_len_idx_t, pow10_mod_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A140274: a(n) = least nonnegative integer m such that 
 * the concatenation of a(1)..a(n-1) followed by a(n)=m is ≡ 1 (mod n).
 * Offset: 1
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_len_idx_t.h"
#include "../utils/pow10_mod_len.h"

/* -------------------- helpers -------------------- */

/* -------------------- static storage for memoization -------------------- */

static seq_t *a_vals   = NULL;          /* a[1..] values   */
static uint16_t *len_vals = NULL;      /* decimal digit lengths of a[1..] */
static size_t  cap       = 0;           /* allocated capacity (max n stored) */
static idx_t   computed_n = 0;          /* largest n already computed */

/* ensure arrays have room for indices up to n */
static void ensure_capacity(idx_t n)
{
    if (n <= cap) return;
    size_t new_cap = cap ? cap * 2 : 16;
    while (new_cap < (size_t)n) new_cap *= 2;
    a_vals   = (seq_t *)realloc(a_vals,   (new_cap + 1) * sizeof(seq_t));
    len_vals = (uint16_t *)realloc(len_vals, (new_cap + 1) * sizeof(uint16_t));
    cap = new_cap;
}

/* -------------------- main function -------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1; return 0 as fallback */

    /* compute up to n if necessary */
    if (n > computed_n) {
        ensure_capacity(n);

        /* base case a(1) = 1 */
        if (computed_n < 1) {
            a_vals[1]   = (seq_t)1;
            len_vals[1] = 1;
            computed_n = 1;
        }

        /* compute terms from computed_n+1 up to n */
        for (idx_t k = computed_n + 1; k <= n; ++k) {

            /* remainder of concatenation of a[1..k-1] modulo k */
            idx_t rem = 0;
            for (idx_t i = 1; i < k; ++i) {
                int len_i = len_vals[i];
                idx_t pow10_i = pow10_mod_len((idx_t)len_i, k);
                idx_t ai_mod = (idx_t)(a_vals[i] % (seq_t)k);  /* k small */
                rem = (rem * pow10_i + ai_mod) % k;
            }

            /* find least m >= 0 such that (rem * 10^len(m) + m) % k == 1 */
            idx_t m = 0;
            while (1) {
                int len_m = digit_len_idx_t(m);
                idx_t pow10_m = pow10_mod_len((idx_t)len_m, k);
                idx_t val = (rem * pow10_m + (m % k)) % k;
                if (val == 1) break;
                ++m;
            }

            a_vals[k]   = (seq_t)m;
            len_vals[k] = (uint16_t)digit_len_idx_t(m);
        }

        computed_n = n;
    }

    return a_vals[n];
}
