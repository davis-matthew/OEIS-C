/*
 * A169908  a(n) = n*n in carryless digital root arithmetic in base 10.
 * https://oeis.org/A169908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n*n in carryless digital‑root arithmetic (base 10)
 * Range:     u8 n <= 19, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 55 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Digital root product of two digits (each 0‑9).  Returns 0‑9. */
static inline seq_t dr_prod(seq_t a, seq_t b)
{
    if (a == (seq_t)0 || b == (seq_t)0) return (seq_t)0;
    seq_t p = a * b;                /* 0 ≤ p ≤ 81 */
    seq_t r = p % (seq_t)9;
    return r == (seq_t)0 ? (seq_t)9 : r;
}

/* Digital root sum of two digits (each 0‑9).  Returns 0‑9. */
static inline seq_t dr_sum(seq_t a, seq_t b)
{
    seq_t s = a + b;
    if (s == (seq_t)0) return (seq_t)0;
    seq_t r = s % (seq_t)9;
    return r == (seq_t)0 ? (seq_t)9 : r;
}

/* Square of n in carryless digital‑root arithmetic. */
static seq_t carryless_square(seq_t n)
{
    if (n == (seq_t)0) return (seq_t)0;

    /* Extract decimal digits of n (least‑significant first). */
    size_t cap = 8, len = 0;
    uint8_t *digits = (uint8_t*)malloc(cap * sizeof(uint8_t));
    if (!digits) return (seq_t)0;

    seq_t tmp = n;
    while (tmp != (seq_t)0) {
        if (len == cap) {
            cap <<= 1;
            uint8_t *tmp2 = (uint8_t*)realloc(digits, cap * sizeof(uint8_t));
            if (!tmp2) { free(digits); return (seq_t)0; }
            digits = tmp2;
        }
        digits[len++] = (uint8_t)(tmp % (seq_t)10);
        tmp /= (seq_t)10;
    }

    /* Result may have at most 2*len digits. */
    size_t res_cap = len + len;
    uint8_t *res = (uint8_t*)calloc(res_cap, sizeof(uint8_t));
    if (!res) { free(digits); return (seq_t)0; }

    /* Carry‑less multiplication (square). */
    for (size_t i = 0; i < len; ++i) {
        for (size_t j = 0; j < len; ++j) {
            seq_t p = dr_prod(digits[i], digits[j]);   /* 0 ≤ p ≤ 9 */
            seq_t s = dr_sum(res[i + j], p);           /* 0 ≤ s ≤ 9 */
            res[i + j] = (uint8_t)s;
        }
    }

    /* Find most significant non‑zero digit. */
    size_t ms = 0;
    for (size_t k = 0; k < res_cap; ++k) {
        if (res[k] != 0) ms = k;
    }

    /* Convert digit array back to seq_t (most significant first). */
    seq_t result = (seq_t)0;
    for (size_t k = ms; ; --k) {
        result = result * (seq_t)10 + (seq_t)res[k];
        if (k == 0) break;
    }

    free(digits);
    free(res);
    return result;
}

seq_t a(idx_t n)
{
    return carryless_square((seq_t)n);
}
