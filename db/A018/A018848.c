/*
 * A018848  Strobogrammatic squares: the same upside down (probably finite).
 * https://oeis.org/A018848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   cmp_seq_t, floor_sqrt, seq_from_str, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for strobogrammatic squares */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* External utilities provided by the build environment */
#include "../utils/seq_mul.h"
#include "../utils/floor_sqrt.h"
#include "../utils/seq_from_str.h"
#include "../utils/cmp_seq_t.h"

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
        seq_t *tmp = (seq_t *)realloc(squares, new_cap * sizeof(seq_t));
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
        qsort(squares, squares_len, sizeof(seq_t), cmp_seq_t);
        squares_unsorted = false;
    }

    return squares[n - 1];
}
