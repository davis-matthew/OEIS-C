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
 * Helpers:   digit_product, digit_sum, floor_sqrt, grow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A117680 */
/* Sequence: squares whose sum of digits, product of digits,
   digital root and multiplicative digital root are all squares.
   Computed by exhaustive search over squares. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_mul.h"
#include "../utils/floor_sqrt.h"
#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"
#include "../utils/grow_seq.h"

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
