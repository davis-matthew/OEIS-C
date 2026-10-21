/*
 * A219031  Table read by rows: n-th row lists all distinct subwords of
 *          decimal representation of n-th square.
 * https://oeis.org/A219031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, base, look
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 2820, u32 n <= 9016, u64 n <= 9016,
 *            u128 n <= 9016, big any n
 * Helpers:   cmp_seq_t, ensure_capacity, get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A219031
 *   Table read by rows: n‑th row lists all distinct subwords of
 *   decimal representation of n‑th square, sorted in increasing order.
 *   The sequence a(n) is the n‑th element when the rows are concatenated
 *   row by row.  This implementation handles arbitrary‑precision seq_t
 *   and uint64_t seq_t.  It uses caching to support random access.
 *
 * Formula: a(n) = i-th element of the flattened list of sorted distinct
 * subwords of the decimal digits of m², where m is the row number.
 */

#include <stdlib.h>
#include <stdbool.h>

/* utility helpers from the build system */
#include "../utils/get_digits.h"
#include "../utils/ensure_capacity.h"
#include "../utils/cmp_seq_t.h"

/* external function required by the build system */
seq_t a(idx_t n)
{
    /* cache of already computed sequence values     */
    static seq_t *cache = NULL;      /* pointer to allocated array      */
    static size_t cache_size  = 0;   /* number of values stored         */
    static size_t cache_limit = 0;   /* allocated capacity              */

    /* next square number whose row has not yet been processed */
    static idx_t next_square = 0;

    /* local buffer for digits of processed square
     * (get_digits returns digits least significant first)    */
    static uint8_t digit_buf[32];
    static uint8_t rev_buf[32];      /* digits in big‑endian order     */
    size_t digit_len;

    while (cache_size <= (size_t)n) {
        /* compute the square of the current index */
        seq_t square = (seq_t)next_square * (seq_t)next_square;

        /* obtain decimal digits of the square (least significant first) */
        get_digits(square, digit_buf, &digit_len);

        /* reverse the digits to get most significant first order   */
        for (size_t k = 0; k < digit_len; ++k)
            rev_buf[k] = digit_buf[digit_len - 1 - k];

        /* gather distinct subwords (numeric values) */
        seq_t sub_words[128];          /* sufficient for ≥ 200 subwords   */
        size_t sub_cnt = 0;

        for (size_t i = 0; i < digit_len; ++i) {
            seq_t val = 0;
            for (size_t j = i; j < digit_len; ++j) {
                val = val * 10 + (seq_t)rev_buf[j];
                /* check uniqueness in the current row */
                bool exists = false;
                for (size_t k = 0; k < sub_cnt; ++k)
                    if (sub_words[k] == val) { exists = true; break; }
                if (!exists)
                    sub_words[sub_cnt++] = val;
            }
        }

        /* sort the subwords in increasing numeric order */
        qsort(sub_words, sub_cnt, sizeof(seq_t), cmp_seq_t);

        /* ensure the global cache can hold the new entries */
        size_t required = cache_size + sub_cnt;
        if (required > cache_limit) {
            void *tmp = cache;
            ensure_capacity(required, &cache_limit, &tmp, sizeof(seq_t));
            cache = (seq_t *)tmp;
        }

        /* append the sorted row into the cache */
        for (size_t i = 0; i < sub_cnt; ++i)
            cache[cache_size++] = sub_words[i];

        /* move on to the next square */
        ++next_square;
    }

    return cache[(size_t)n];
}
