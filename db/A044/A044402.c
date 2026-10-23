/*
 * A044402  Numbers n such that string 7,0 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044402: the n‑th integer containing the substring “70” in its
 * decimal expansion, while n‑1 does not contain that substring.
 * Computation is performed by scanning integers from 1 upwards, keeping
 * a cache of found terms. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/get_digits.h"

static inline bool has_70(seq_t x)
{
    uint8_t buf[256];
    size_t len;
    get_digits(x, buf, &len);

    /* get_digits returns digits least‑significant first;
       the substring “70” appears as 0 followed by 7 in that order. */
    for (size_t i = 0; i + 1 < len; ++i)
        if (buf[i] == 0 && buf[i + 1] == 7)
            return true;
    return false;
}

typedef struct {
    seq_t *data;
    size_t size;
    size_t cap;
} cache_t;

static cache_t a_cache = { NULL, 0, 0 };
static seq_t a_next = 0;   /* next candidate to examine */

/* Grow the cache when necessary */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= a_cache.cap)
        return;
    size_t new_cap = a_cache.cap ? a_cache.cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    a_cache.data = (seq_t *)realloc(a_cache.data, new_cap * sizeof(seq_t));
    a_cache.cap = new_cap;
}

/* Generate terms up to the target index */
static void generate_up_to(idx_t target)
{
    while (a_cache.size < (size_t)target) {
        a_next = a_next + (seq_t)1;          /* next number to test   */
        if (has_70(a_next) && !has_70(a_next - (seq_t)1)) {
            ensure_cache_capacity(a_cache.size + 1);
            a_cache.data[a_cache.size++] = a_next;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;     /* undefined for offset 1, but harmless */
    generate_up_to(n);
    return a_cache.data[n - 1];
}
