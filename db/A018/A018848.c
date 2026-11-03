/*
 * A018848  Strobogrammatic squares: the same upside down (probably finite).
 * https://oeis.org/A018848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for strobogrammatic squares */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* External utilities provided by the build environment */
#include <stdint.h>
#include <string.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

/* Forward declarations for helper functions */
static void push_square(seq_t val);
static void gen_rec(int len, char *buf, int l, int r);
static void process(const char *s);

/* Cache of found strobogrammatic squares */
static seq_t *squares = NULL;
static size_t squares_len = 0;
static size_t squares_cap = 0;
static bool squares_unsorted = true;

/* Next length of strobogrammatic numbers to generate */
static int next_len = 1;

/* Helper to add a value to the cache, resizing when needed */
static void push_square(seq_t val)
{
    if (squares_len == squares_cap) {
        size_t new_cap = squares_cap ? squares_cap * 2 : 16;
        seq_t *tmp = mem_resize(seq_t, squares, new_cap);
        if (!tmp) abort();      /* unrecoverable allocation failure */
        squares = tmp;
        squares_cap = new_cap;
    }
    squares[squares_len++] = val;
    squares_unsorted = true;
}

/* Process a generated strobogrammatic string: test if it is a square */
static void process(const char *s)
{
    seq_t v = seq_from_str(s);
    seq_t r = floor_sqrt(v);
    seq_t sq = seq_mul(r, r);
    if (sq == v) {
        push_square(v);
    }
}

/* Recursive generation of strobogrammatic numbers of a given length */
static void gen_rec(int len, char *buf, int l, int r)
{
    if (l > r) {
        buf[len] = '\0';
        process(buf);
        return;
    }
    if (l == r) {
        static const char mids[3] = {'0', '1', '8'};
        for (int i = 0; i < 3; ++i) {
            buf[l] = mids[i];
            gen_rec(len, buf, l + 1, r - 1);
        }
        return;
    }
    static const char pairs[5][2] = {
        {'0', '0'}, {'1', '1'}, {'6', '9'},
        {'8', '8'}, {'9', '6'}
    };
    for (int i = 0; i < 5; ++i) {
        if (l == 0 && pairs[i][0] == '0')
            continue;  /* leading zero not allowed for length > 1 */
        buf[l] = pairs[i][0];
        buf[r] = pairs[i][1];
        gen_rec(len, buf, l + 1, r - 1);
    }
}

/* Public function: returns the n-th strobogrammatic square (1‑based) */
seq_t a(idx_t n)
{
    /* Ensure we have at least n values in the cache */
    while (squares_len < n) {
        /* Generate all strobogrammatic numbers of length next_len */
        char *buf = (char *)malloc((size_t)next_len + 1);
        if (!buf) abort();   /* unrecoverable failure */
        gen_rec(next_len, buf, 0, next_len - 1);
        free(buf);
        ++next_len;
    }

    /* Sort once after new elements are added */
    if (squares_unsorted) {
        mem_qsort(squares, squares_len, sizeof(seq_t), cmp_seq_t);
        squares_unsorted = false;
    }

    return squares[n - 1];
}
