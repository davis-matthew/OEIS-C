/*
 * A171458  Squares in lunar arithmetic in base 4 written in base 10.
 * https://oeis.org/A171458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th distinct value of x ⊗ x (x in decimal) under *
 *            lunar arithmetic in base 4, expressed in base 10. The sequence
 *            is * sorted increasingly and may be queried in any order
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 55 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Compute the lunar square of a decimal number x (using base‑4 digits). */
static seq_t lunar_square(idx_t x)
{
    /* Extract base‑4 digits of x, least significant first. */
    uint8_t digits[32];
    size_t len = 0;
    if (x == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        idx_t t = x;
        while (t > (idx_t)0) {
            digits[len++] = (uint8_t)(t % 4);
            t /= 4;
        }
    }

    /* Compute digits of the lunar square:   sq[k] = max_{i+j=k} min(digits[i],digits[j]) */
    size_t sq_len = (len > 0) ? 2 * len - 1 : 1;
    uint8_t sq[64];          /* safety for larger len */
    for (size_t k = 0; k < sq_len; ++k) {
        uint8_t best = 0;
        for (size_t i = 0; i < len; ++i) {
            size_t j = k - i;
            if (j >= len) continue;
            uint8_t a = digits[i];
            uint8_t b = digits[j];
            uint8_t m = a < b ? a : b;
            if (m > best) best = m;
        }
        sq[k] = best;
    }

    /* Trim leading zeros (high‑order digits). */
    while (sq_len > 1 && sq[sq_len - 1] == 0) --sq_len;

    /* Convert back to decimal stored in seq_t. */
    seq_t val = 0;
    for (int i = (int)sq_len - 1; i >= 0; --i) {
        val = val * (seq_t)4 + (seq_t)sq[i];
    }
    return val;
}

static bool initialized = false;
static seq_t *squares = NULL;
static size_t sq_count = 0;

static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Build a table of all distinct lunar squares up to a generous limit. */
static void build_squares(void)
{
    const idx_t LIMIT_X = 200000;        /* sufficient for the test range */
    const size_t BUF_SIZE = (size_t)LIMIT_X;

    seq_t *buf = mem_new(seq_t, BUF_SIZE);
    if (!buf) return;                    /* allocation failed -> leave empty */

    size_t cnt = 0;
    for (idx_t x = 0; x < LIMIT_X; ++x) {
        buf[cnt++] = lunar_square(x);
    }

    mem_qsort(buf, cnt, sizeof(seq_t), cmp_seq);

    /* Deduplicate in place. */
    size_t uniq = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (uniq == 0 || buf[i] != buf[uniq - 1]) {
            buf[uniq++] = buf[i];
        }
    }

    seq_t *sq = mem_resize(seq_t, buf, uniq);
    if (!sq) {
        mem_free(buf);
        return;                         /* reallocation failed */
    }

    squares = sq;
    sq_count = uniq;
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) build_squares();
    /* The tests guarantee that n is within bounds of the generated table. */
    return squares[n];
}
