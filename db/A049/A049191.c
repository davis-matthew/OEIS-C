/*
 * A049191  Start with 1. Convert to base 3, describe it in base 3, convert to
 *          base 3.
 * https://oeis.org/A049191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 9, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A049191:  a(1) = 1; for n > 1, a(n) is obtained from a(n‑1) by:  
   1) writing a(n‑1) in base 3,  
   2) describing the runs of identical digits in base 3,  
   3) interpreting that description as a base‑3 integer. */

#include <stdlib.h>
#include <stdint.h>

/* dynamic array for small base‑3 digits (0,1,2) */
typedef struct {
    uint8_t *data;
    size_t   len;
    size_t   cap;
} dyn_t;

static void dyn_init(dyn_t *d) { d->data = NULL; d->len = d->cap = 0; }

static void dyn_push(dyn_t *d, uint8_t v)
{
    if (d->len == d->cap) {
        size_t new_cap = d->cap ? d->cap * 2 : 16;
        d->data = (uint8_t *)realloc(d->data, new_cap);
        d->cap = new_cap;
    }
    d->data[d->len++] = v;
}

static void dyn_free(dyn_t *d) { free(d->data); d->len = d->cap = 0; d->data = NULL; }

/* append the base‑3 representation of a small unsigned integer to the output */
static void append_uint_base3(size_t v, dyn_t *out)
{
    if (v == 0) { dyn_push(out, 0); return; }
    uint8_t stack[64];
    size_t sp = 0;
    while (v > 0) {
        stack[sp++] = (uint8_t)(v % 3);
        v /= 3;
    }
    for (size_t i = 0; i < sp; ++i)
        dyn_push(out, stack[sp - 1 - i]);
}

/* obtain the digits of a number in base‑3, most‑significant digit first */
static void get_digits_base3(seq_t x, dyn_t *out)
{
    while (x > (seq_t)0) {
        uint8_t rem = (uint8_t)(x % (seq_t)3);
        dyn_push(out, rem);                /* store LSB first temporarily */
        x = x / (seq_t)3;
    }
    /* reverse to obtain normal order */
    for (size_t i = 0, j = out->len - 1; i < j; ++i, --j) {
        uint8_t tmp = out->data[i];
        out->data[i] = out->data[j];
        out->data[j] = tmp;
    }
}

seq_t a(idx_t n)
{
    /* memoization of computed terms */
    static seq_t      *cache = NULL;
    static idx_t       cache_size = 0;

    if (n == 1) {
        if (cache_size < 1) {
            cache = (seq_t *)realloc(cache, sizeof(seq_t));
            cache[0] = (seq_t)1;
            cache_size = 1;
        }
        return (seq_t)1;
    }

    /* extend cache up to n */
    if (cache_size < n) {
        cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        for (idx_t k = cache_size; k < n; ++k) {
            seq_t cur = cache[k - 1];

            dyn_t orig, out;
            dyn_init(&orig);
            dyn_init(&out);

            get_digits_base3(cur, &orig);

            size_t i = 0;
            while (i < orig.len) {
                uint8_t dig = orig.data[i];
                size_t cnt = 0;
                while (i < orig.len && orig.data[i] == dig) {
                    ++cnt;
                    ++i;
                }
                append_uint_base3(cnt, &out);
                dyn_push(&out, dig);
            }

            seq_t newval = (seq_t)0;
            for (size_t j = 0; j < out.len; ++j)
                newval = newval * (seq_t)3 + (seq_t)out.data[j];

            dyn_free(&orig);
            dyn_free(&out);

            cache[k] = newval;
        }
        cache_size = n;
    }

    return cache[n - 1];
}
