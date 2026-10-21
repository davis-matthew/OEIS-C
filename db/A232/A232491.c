/*
 * A232491  Numbers k such that 10^k is not of the form m + sum of digits of
 *          m.
 * https://oeis.org/A232491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th integer k for which no m satisfies m +
 *            sumdigits(m) = 10^k. This is tested by trying all possible u =
 *            10^k - m and checking the equivalent condition u + sumdigits(u)
 *            = 9k - 1
 * Range:     u8 n <= 25, u16 n <= 6408, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* sum of decimal digits of a 64‑bit unsigned integer */
static inline seq_t digit_sum_u64(uint64_t v)
{
    seq_t s = 0;
    while (v) {
        s += v % 10;
        v /= 10;
    }
    return s;
}

/* test whether 10^k can be written as m + sumdigits(m) */
static inline bool has_solution_idx(idx_t k)
{
    uint64_t target = 9ULL * (uint64_t)k - 1;       /* 9k – 1 */
    /* number of decimal digits of target */
    uint64_t t = target;
    unsigned digs = 0;
    do { digs++; t /= 10; } while (t);
    uint64_t max_sum = 9ULL * digs;                /* maximum possible digit sum */

    for (uint64_t s = 0; s <= max_sum; ++s) {
        uint64_t u = target - s;
        if (digit_sum_u64(u) == (seq_t)s) /* u + sumdigits(u) = target ? */
            return true;                 /* solution exists */
    }
    return false;                        /* no solution */
}

/* sequence A232491 */
seq_t a(idx_t n)
{
    static seq_t *table = NULL;
    static size_t table_len = 0;
    static size_t table_cap = 0;
    static idx_t last_k = 0;            /* last checked k */

    /* extend the table until we have at least n terms */
    while (table_len < (size_t)n) {
        /* find next k that has no solution */
        do { ++last_k; } while (has_solution_idx(last_k));

        /* allocate / grow storage as needed */
        if (table_len == table_cap) {
            size_t new_cap = table_cap ? table_cap * 2 : 128;
            seq_t *tmp = (seq_t *)realloc(table, new_cap * sizeof(seq_t));
            if (!tmp) return 0;        /* allocation failed – return 0 as error */
            table = tmp;
            table_cap = new_cap;
        }
        table[table_len++] = (seq_t)last_k;
    }

    return table[(size_t)n - 1];
}
