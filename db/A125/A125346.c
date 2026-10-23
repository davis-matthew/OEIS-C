/*
 * A125346  Number of base 9 circular n-digit numbers with adjacent digits
 *          differing by 4 or less.
 * https://oeis.org/A125346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) where A is the 9×9 adjacency matrix * for the
 *            directed graph on vertices 0–8 with an edge i→j iff * |i−j| ≤ 4.
 *            This counts n‑digit circular base‑9 numbers whose * adjacent
 *            digits differ by at most 4 (including the last‑to‑first *
 *            adjacency). a(0)=1, a(1)=9, a(2)=61, … (OEIS A125...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const seq_t ADJ[9][9] = {
    {1,1,1,1,1,0,0,0,0},  /* 0 connects to 0‑4 */
    {1,1,1,1,1,1,0,0,0},  /* 1 connects to 0‑5 */
    {1,1,1,1,1,1,1,0,0},  /* 2 connects to 0‑6 */
    {1,1,1,1,1,1,1,1,0},  /* 3 connects to 0‑7 */
    {1,1,1,1,1,1,1,1,1},  /* 4 connects to 0‑8 */
    {0,1,1,1,1,1,1,1,1},  /* 5 connects to 1‑8  (note the trailing 1) */
    {0,0,1,1,1,1,1,1,1},  /* 6 connects to 2‑8 */
    {0,0,0,1,1,1,1,1,1},  /* 7 connects to 3‑8 */
    {0,0,0,0,1,1,1,1,1}   /* 8 connects to 4‑8 */
};

/* matrix multiplication: res = a · b  (9×9 matrices) */
static void mat_mul(seq_t res[9][9], const seq_t a[9][9], const seq_t b[9][9])
{
    seq_t tmp[9][9];
    for (size_t i = 0; i < 9; ++i)
        for (size_t j = 0; j < 9; ++j) {
            seq_t sum = (seq_t)0;
            for (size_t k = 0; k < 9; ++k)
                sum += a[i][k] * b[k][j];
            tmp[i][j] = sum;
        }
    for (size_t i = 0; i < 9; ++i)
        for (size_t j = 0; j < 9; ++j)
            res[i][j] = tmp[i][j];
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;           /* empty string */

    /* result = identity matrix */
    seq_t res[9][9];
    for (size_t i = 0; i < 9; ++i)
        for (size_t j = 0; j < 9; ++j)
            res[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    /* base = adjacency matrix A */
    seq_t base[9][9];
    for (size_t i = 0; i < 9; ++i)
        for (size_t j = 0; j < 9; ++j)
            base[i][j] = ADJ[i][j];

    idx_t e = n;
    while (e > 0) {
        if (e & 1) {
            seq_t temp[9][9];
            mat_mul(temp, res, base);
            for (size_t i = 0; i < 9; ++i)
                for (size_t j = 0; j < 9; ++j)
                    res[i][j] = temp[i][j];
        }
        seq_t sq[9][9];
        mat_mul(sq, base, base);
        for (size_t i = 0; i < 9; ++i)
            for (size_t j = 0; j < 9; ++j)
                base[i][j] = sq[i][j];
        e >>= 1;
    }

    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < 9; ++i)
        sum += res[i][i];
    return sum;
}
