/*
 * A124958  Number of base 30 circular n-digit numbers with adjacent digits
 *          differing by 2 or less.
 * https://oeis.org/A124958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A124958
    Number of base 30 circular n‑digit numbers with adjacent digits differing by 2 or less.
    The value equals trace(A^n) where A is the 30×30 adjacency matrix with
    A[i][j] = 1 iff |i−j| ≤ 2.  For n = 0 we return 1 by definition. */

#include <stddef.h>
#include <stdbool.h>

#define B 30   /* size of the digit alphabet */

/* multiply two B×B matrices: c = a × b */
static void mul(const seq_t a[B][B], const seq_t b[B][B], seq_t c[B][B])
{
    for (int i = 0; i < B; ++i) {
        for (int j = 0; j < B; ++j) {
            seq_t sum = (seq_t)0;
            for (int k = 0; k < B; ++k)
                sum += a[i][k] * b[k][j];
            c[i][j] = sum;
        }
    }
}

/* Compute a(idx_t n).  a(0) = 1. */
seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;

    /* Build adjacency matrix once. */
    static seq_t base[B][B];
    static bool built = false;
    if (!built) {
        for (int i = 0; i < B; ++i)
            for (int j = 0; j < B; ++j)
                base[i][j] = ((i > j) ? (i - j) : (j - i)) <= 2 ? (seq_t)1 : (seq_t)0;
        built = true;
    }

    /* Identity matrix for the result. */
    seq_t res[B][B];
    for (int i = 0; i < B; ++i)
        for (int j = 0; j < B; ++j)
            res[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    /* Temporary matrices. */
    seq_t power[B][B];
    seq_t tmp[B][B];
    for (int i = 0; i < B; ++i)
        for (int j = 0; j < B; ++j)
            power[i][j] = base[i][j];

    /* Binary exponentiation. */
    idx_t exp = n;
    while (exp != 0u) {
        if (exp & 1u) {
            mul(res, power, tmp);
            for (int i = 0; i < B; ++i)
                for (int j = 0; j < B; ++j)
                    res[i][j] = tmp[i][j];
        }
        mul(power, power, tmp);
        for (int i = 0; i < B; ++i)
            for (int j = 0; j < B; ++j)
                power[i][j] = tmp[i][j];
        exp >>= 1;
    }

    /* Sum diagonal entries for trace. */
    seq_t trace = (seq_t)0;
    for (int i = 0; i < B; ++i)
        trace += res[i][i];

    return trace;
}
