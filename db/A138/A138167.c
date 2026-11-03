/*
 * A138167  Numbers containing their length in ternary representation.
 * https://oeis.org/A138167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9802 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth integer m whose base‑3 representation contains
   the base‑3 representation of its length (L) as a consecutive substring. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static bool contains_len_pattern(seq_t m)
{
    /* Collect digits of m in reverse order */
    uint8_t rev[256];
    size_t len = 0;
    seq_t tmp = m;
    while (tmp > 0) {
        rev[len++] = (uint8_t)(tmp % 3);
        tmp /= 3;
    }
    if (len == 0) return false;            /* m == 0 is not tested */

    /* Reorder digits to most‑significant first */
    uint8_t digits[256];
    for (size_t i = 0; i < len; ++i)
        digits[i] = rev[len - 1 - i];

    /* Length of the ternary representation */
    seq_t L = (seq_t)len;

    /* Base‑3 representation of L (also in reverse) */
    uint8_t pat_rev[32];
    size_t patlen = 0;
    seq_t t = L;
    while (t > 0) {
        pat_rev[patlen++] = (uint8_t)(t % 3);
        t /= 3;
    }
    /* Reorder pattern to normal order */
    uint8_t pat[32];
    for (size_t i = 0; i < patlen; ++i)
        pat[i] = pat_rev[patlen - 1 - i];

    if (patlen > len) return false;

    /* Sliding window search for pattern */
    for (size_t i = 0; i + patlen <= len; ++i) {
        bool ok = true;
        for (size_t j = 0; j < patlen; ++j) {
            if (digits[i + j] != pat[j]) { ok = false; break; }
        }
        if (ok) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 1;
    for (;;) {
        if (contains_len_pattern((seq_t)m)) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
        ++m;
    }
}
