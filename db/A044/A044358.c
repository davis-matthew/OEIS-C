/*
 * A044358  Numbers n such that string 2,6 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 712, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 55 terms through n = 4849 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044358: a(n) = nth integer whose decimal representation contains
   substring "26" while (n-1) does not.  This implementation
   computes the terms on demand and memoises them so that calls may
   arrive in any order. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Static cache of discovered terms */
static seq_t *cache = NULL;   /* array of sequence values   */
static size_t count = 0;      /* number of terms stored    */
static size_t capacity = 0;   /* allocated capacity        */

/* Test whether decimal string of x contains "26" */
static bool has26(seq_t x)
{
    /* A buffer big enough for the decimal representation of any
       seq_t value that appears for the requested n values.  The
       sequence grows much slower than the range of seq_t so 4096
       bytes is ample. */
    static char buf[4096];
    size_t len;

    seq_to_str(x, buf, &len);

    for (size_t i = 0; i + 1 < len; ++i)
        if (buf[i] == '2' && buf[i + 1] == '6')
            return true;

    return false;
}

/* Ensure that the cache contains at least 'target' terms. */
static void extend_cache(size_t target)
{
    if (target <= count)
        return;

    /* Grow the array if necessary. */
    if (capacity < target) {
        size_t new_cap = capacity ? capacity * 2 : 16;
        if (new_cap < target)
            new_cap = target;
        seq_t *tmp = seq_realloc(cache, capacity, new_cap);
        cache = tmp;
        capacity = new_cap;
    }

    /* Scan forward from the next integer after the last cached value. */
    static seq_t last_checked = 0;
    seq_t cand = last_checked + 1;

    while (count < target) {
        bool cand_has = has26(cand);
        bool prev_has = has26(cand - 1);
        if (cand_has && !prev_has)
            cache[count++] = cand;
        cand = cand + 1;
    }

    last_checked = cand - 1;
}

/* Public interface: returns the nth term of A044358. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1, but safe return */
        return (seq_t)0;

    extend_cache((size_t)n);
    return cache[(size_t)n - 1];
}
