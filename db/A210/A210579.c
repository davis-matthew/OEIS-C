/*
 * A210579  a(1)=2, a(n+1)=a(n)*(n-th digit of the sequence or 10 in case of
 *          digit '0').
 * https://oeis.org/A210579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 2, a(n+1) = a(n) * d, where d is the n-th digit of the *
 *            concatenated decimal representation of the sequence, and d = 10
 *            if the * digit is 0. Saves previously computed terms and digits
 *            to allow * out‑of‑order queries
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 28 (2026-11-03)
 * Helpers:   mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include <stdbool.h>
#include <string.h>
#include "../utils/num_digits.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Static buffers for memoisation */
static seq_t *values = NULL;
static size_t values_cap = 0;
static size_t values_len = 0;

static uint8_t *digits = NULL;
static size_t digits_cap = 0;
static size_t digits_len = 0;

/* Append the decimal digits of v to the global digit buffer. */
static void append_digits(seq_t v)
{
    size_t dlen = num_digits(v);
    char *buf = (char *)malloc(dlen + 1);
    if (!buf) return;   /* allocation failure; silently ignore – caller will fail */
    seq_to_str(v, buf, &dlen);

    ensure_capacity(digits_len + dlen, &digits_cap, OEISC_VPP(&digits), sizeof(uint8_t));
    for (size_t i = 0; i < dlen; ++i) {
        digits[digits_len + i] = (uint8_t)(buf[i] - '0');
    }
    digits_len += dlen;
    free(buf);
}

/* Compute a(n) for any n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for the used ranges */

    /* Initialise first term once. */
    if (values_len == 0) {
        seq_t first = (seq_t)2;
        ensure_capacity(1, &values_cap, OEISC_VPP(&values), sizeof(seq_t));
        values[0] = first;
        values_len = 1;

        /* Initialise digit buffer. */
        digits_len = 0;
        digits_cap = 0;
        append_digits(first);
    }

    /* Extend precomputation to the required index. */
    while (values_len < (size_t)n) {
        seq_t prev = values[values_len - 1];
        uint8_t dig = digits[values_len - 1];
        seq_t mult = (dig == 0 ? (seq_t)10 : (seq_t)dig);
        seq_t next = seq_mul(prev, mult);

        ensure_capacity(values_len + 1, &values_cap, OEISC_VPP(&values), sizeof(seq_t));
        values[values_len] = next;
        values_len++;

        append_digits(next);
    }

    return values[n - 1];
}
