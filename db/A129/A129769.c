/*
 * A129769  Exponents m(i) for exceptional groups with best guesses for E7 1/2
 *          and E9 added (there is a problem with the dimension of E9 as no
 *          sum of odd numbers will equal the 484, I get 483): triangular
 *          sequence is: A1,G2,F4,E6,E7 E7 1/2,E8,E9.
 * https://oeis.org/A129769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   a(1)=1, a(2)=1, a(3)=5, a(4)=1, a(5)=5, a(6)=7, etc. a(n) is
 *            obtained by listing, in order, the exponents of all exceptional
 *            and conjectural Lie groups as follows: A1 → {1} G2 → {1,5} F4 →
 *            {1,5,7,11} E6 → {1,4,5,7,8,11} E7 → {1,5,7,9,11,13,17} E7½ →
 *            {1,6,9,11,13,15,17,19} E8 → {1,7,11,13,17,19,23...
 * Range:     u8 n <= 45, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t row0[] = {1};
static const seq_t row1[] = {1,5};
static const seq_t row2[] = {1,5,7,11};
static const seq_t row3[] = {1,4,5,7,8,11};
static const seq_t row4[] = {1,5,7,9,11,13,17};
static const seq_t row5[] = {1,6,9,11,13,15,17,19};
static const seq_t row6[] = {1,7,11,13,17,19,23,29};
static const seq_t row7[] = {1,11,17,19,23,29,31,51,55};

typedef struct {
    size_t len;
    const seq_t *data;
} row_t;

static const row_t rows[] = {
    {1, row0},
    {2, row1},
    {4, row2},
    {6, row3},
    {7, row4},
    {8, row5},
    {8, row6},
    {9, row7}
};
static const size_t n_rows = sizeof(rows) / sizeof(rows[0]);

seq_t a(idx_t n)
{
    /* Indices in the problem are 1‑based. */
    idx_t idx = n;
    idx_t offset = 0;

    for (size_t i = 0; i < n_rows; ++i) {
        idx_t row_len = (idx_t)rows[i].len;
        if (idx <= offset + row_len) {
            /* Convert to zero‑based index for the array. */
            idx_t pos = idx - offset - (idx_t)1;
            return rows[i].data[(size_t)pos];
        }
        offset += row_len;
    }
    /* If n is beyond the known rows, return 0. */
    return (seq_t)0;
}
