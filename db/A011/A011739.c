/*
 * A011739  A binary m-sequence: expansion of reciprocal of x^26 + x^8 + x^7 +
 *          x + 1 (mod 2, shifted by 25 initial 0's).
 * https://oeis.org/A011739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = value of the 26‑term binary recurrence * a(n) = a(n‑1)
 *            XOR a(n‑7) XOR a(n‑8) XOR a(n‑26), * with shift of 25 zeroes
 *            (a(0)..a(24)=0)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a static cache of the recurrence
 * values s(k) = coefficient of x^k in 1/(x^26+x^8+x^7+x+1) over GF(2).
 * The returned a(n) is simply s(n‑25) for n ≥ 25, otherwise 0. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static size_t cached_len = 0;
static uint8_t *cached_vals = NULL;   /* cache of s(k) (0 or 1) */

/* Ensure the cache contains values up to idx (inclusive). */
static void ensure_cache(size_t idx)
{
    if (cached_len > idx)
        return;

    size_t new_len = (idx + 1) * 2;
    uint8_t *new_buf = (uint8_t *)realloc(cached_vals, new_len);
    if (!new_buf) {
        /* Allocation failed; in contest environments abort. */
        abort();
    }
    cached_vals = new_buf;

    /* Initialize new entries */
    for (size_t i = cached_len; i <= idx; ++i) {
        uint8_t val = 0;
        if (i == 0) {
            val = 1;                    /* s_0 = 1 */
        } else {
            /* s_i = s_{i‑1} XOR s_{i‑7} XOR s_{i‑8} XOR s_{i‑26}  (GC 2) */
            uint8_t a = cached_vals[i - 1];
            uint8_t b = (i >= 7)   ? cached_vals[i - 7]   : 0;
            uint8_t c = (i >= 8)   ? cached_vals[i - 8]   : 0;
            uint8_t d = (i >= 26)  ? cached_vals[i - 26]  : 0;
            val = a ^ b ^ c ^ d;
        }
        cached_vals[i] = val;
    }
    cached_len = idx + 1;
}

seq_t a(idx_t n)
{
    if (n < 25) return (seq_t)0;          /* shifted zeros */

    idx_t k = n - 25;                     /* index in s(k) */
    ensure_cache((size_t)k);
    return (seq_t)cached_vals[(size_t)k];
}
