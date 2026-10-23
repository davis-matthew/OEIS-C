/*
 * A046542  Odd numbers among the numerators of the 1/3-Pascal triangle (by
 *          row).
 * https://oeis.org/A046542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = nth odd numerator appearing in the 1/3-Pascal triangle,
 *   read by rows.  Rows are generated using the rule that each
 *   interior element is the sum of the two elements above it,
 *   reduced to lowest terms (denominators are powers of 3).
 *
 *   The first few rows (offset 1) are:
 *      1/1
 *      1/1 1/1
 *      1/1 1/3 1/1
 *      1/1 4/3 4/3 1/1
 *      ...
 *
 *   The function builds the triangle lazily and caches all odd
 *   numerators found so far, allowing out‑of‑order calls to a(n).
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow3.h"

seq_t a(idx_t n) {
    /* static cache of odd numerators */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* the most recently generated row (numerator and exponent of 3) */
    static seq_t *prev_num = NULL;    /* numerator */
    static uint8_t *prev_exp = NULL;  /* exponent of 3 in denominator */
    static size_t prev_len = 0;       /* length of previous row */

    /* index of the next row to generate (0‑based row number) */
    static idx_t next_row = 3;       /* rows 0,1,2 are pre‑initialised */

    /* initialise on first call */
    if (!cache) {
        /* cache at least 8 entries */
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out of memory */

        /* pre‑filled first 6 odd numerators: all equal to 1 */
        cache_len = 6;
        for (size_t i = 0; i < cache_len; ++i)
            cache[i] = (seq_t)1;

        /* prepare previous row: row 2 of the triangle
           fractions: 1/1, 1/3, 1/1  --> numerators 1,1,1 exponents 0,1,0   */
        prev_len = 3;
        prev_num = (seq_t*)malloc(prev_len * sizeof(seq_t));
        prev_exp = (uint8_t*)malloc(prev_len * sizeof(uint8_t));
        if (!prev_num || !prev_exp) return (seq_t)0;
        prev_num[0] = (seq_t)1; prev_exp[0] = 0;
        prev_num[1] = (seq_t)1; prev_exp[1] = 1;
        prev_num[2] = (seq_t)1; prev_exp[2] = 0;
    }

    /* generate rows until we have at least n odd numerators */
    while (cache_len < (size_t)n) {
        size_t row_len = (size_t)next_row + 1;   /* number of elements in the new row */

        /* allocate new row */
        seq_t *new_num = (seq_t*)malloc(row_len * sizeof(seq_t));
        uint8_t *new_exp = (uint8_t*)malloc(row_len * sizeof(uint8_t));
        if (!new_num || !new_exp) return (seq_t)0;

        /* left and right edges */
        new_num[0] = (seq_t)1; new_exp[0] = 0;
        if (row_len > 1) {
            new_num[row_len - 1] = (seq_t)1;
            new_exp[row_len - 1] = 0;
        }

        /* interior elements: sum of two above, reduced to lowest terms */
        for (size_t k = 1; k + 1 < row_len; ++k) {
            seq_t left_num  = prev_num[k - 1];
            uint8_t left_exp  = prev_exp[k - 1];
            seq_t right_num = prev_num[k];
            uint8_t right_exp = prev_exp[k];

            /* compute lcm denominator power of 3 */
            uint8_t maxExp = (left_exp > right_exp) ? left_exp : right_exp;

            /* scale numerators to common denominator */
            seq_t tnum = left_num;
            if (maxExp > left_exp)
                tnum *= pow3(maxExp - left_exp);
            seq_t tnum2 = right_num;
            if (maxExp > right_exp)
                tnum2 *= pow3(maxExp - right_exp);

            seq_t sum = tnum + tnum2;
            uint8_t denomExp = maxExp;

            /* cancel common factors of 3 */
            while (denomExp > 0 && (sum % (seq_t)3 == (seq_t)0)) {
                sum /= (seq_t)3;
                denomExp--;
            }

            new_num[k]  = sum;
            new_exp[k]  = denomExp;
        }

        /* extract odd numerators from the new row and append to cache */
        for (size_t k = 0; k < row_len; ++k) {
            if ((new_num[k] % (seq_t)2) != (seq_t)0) {
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap << 1;
                    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) return (seq_t)0;
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = new_num[k];
            }
        }

        /* recycle previous row buffer */
        free(prev_num);
        free(prev_exp);

        /* set new row as previous for the next iteration */
        prev_num = new_num;
        prev_exp = new_exp;
        prev_len = row_len;
        next_row++;   /* next row index for subsequent generation */
    }

    return cache[(size_t)n - 1];
}
