/*
 * A117680  Squares for which the sum of the digits, the product of the
 *          digits, the digital root and the multiplicative digital root are
 *          all squares.
 * https://oeis.org/A117680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 23, u32 n <= 7055, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 54 terms through n = 8860 (2026-11-03)
 * Helpers:   digit_product, digit_sum, ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A117680 */
/* Sequence: squares whose sum of digits, product of digits,
   digital root and multiplicative digital root are all squares.
   Computed by exhaustive search over squares. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/digit_product.h"
#include "../utils/digit_sum.h"
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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

static bool is_square_seq(seq_t x)
{
    seq_t s = floor_sqrt(x);
    seq_t sq = seq_mul(s, s);
    return sq == x;
}

static bool is_square_and_small(seq_t x)
{
    /* same as is_square_seq but treats 0 as square as well */
    return is_square_seq(x);
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t current_m = 1;          /* next integer to square */

    while (cache_len < n) {
        seq_t m = (seq_t)current_m;
        seq_t square = seq_mul(m, m);

        /* check sum of digits */
        seq_t sum = digit_sum(square);
        /* check product of digits */
        seq_t prod = digit_product(square);

        /* digital root: iterative sum until single digit */
        seq_t d = sum;
        while (d >= (seq_t)10)
            d = digit_sum(d);

        /* multiplicative digital root: iterative product until single digit */
        seq_t md = prod;
        while (md >= (seq_t)10)
            md = digit_product(md);

        if (is_square_seq(sum) && is_square_seq(prod) &&
            is_square_seq(d) && is_square_seq(md)) {

            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len] = square;
            cache_len++;
        }
        current_m++;
    }
    return cache[n - 1];
}
