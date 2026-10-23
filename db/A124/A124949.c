/*
 * A124949  Number of base 21 circular n-digit numbers with adjacent digits
 *          differing by 2 or less.
 * https://oeis.org/A124949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 1, a(n) = trace(A^n) for n >= 1, where A[i][j] = 1 if
 *            |i-j| <= 2, otherwise 0 (0 <= i,j < 21)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define B 21

/* forward declarations */
static void init_adjacency(void);
static void mat_identity(seq_t m[B][B]);
static void mat_copy(const seq_t src[B][B], seq_t dst[B][B]);
static void mat_mul(const seq_t a[B][B], const seq_t b[B][B], seq_t r[B][B]);

/* adjacency matrix A */
static seq_t _adjacency[B][B];
static bool _adjacency_initialized = false;

/* initialize the adjacency matrix once */
static void init_adjacency(void)
{
    if (_adjacency_initialized)
        return;
    for (size_t i = 0; i < B; ++i) {
        for (size_t j = 0; j < B; ++j) {
            size_t diff = (i > j) ? (i - j) : (j - i);
            _adjacency[i][j] = (diff <= 2) ? (seq_t)1 : (seq_t)0;
        }
    }
    _adjacency_initialized = true;
}

/* set matrix to identity */
static void mat_identity(seq_t m[B][B])
{
    for (size_t i = 0; i < B; ++i)
        for (size_t j = 0; j < B; ++j)
            m[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
}

/* copy matrix */
static void mat_copy(const seq_t src[B][B], seq_t dst[B][B])
{
    for (size_t i = 0; i < B; ++i)
        for (size_t j = 0; j < B; ++j)
            dst[i][j] = src[i][j];
}

/* matrix multiplication: r = a * b */
static void mat_mul(const seq_t a[B][B], const seq_t b[B][B], seq_t r[B][B])
{
    for (size_t i = 0; i < B; ++i)
        for (size_t j = 0; j < B; ++j)
            r[i][j] = (seq_t)0;

    for (size_t i = 0; i < B; ++i) {
        for (size_t k = 0; k < B; ++k) {
            if (a[i][k] == (seq_t)0)
                continue;
            const seq_t aik = a[i][k];
            for (size_t j = 0; j < B; ++j) {
                if (b[k][j] == (seq_t)0)
                    continue;
                r[i][j] += aik * b[k][j];
            }
        }
    }
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    init_adjacency();

    seq_t result[B][B];
    seq_t base[B][B];
    seq_t temp[B][B];

    mat_identity(result);
    mat_copy(_adjacency, base);

    idx_t exp = n;
    while (exp) {
        if (exp & (idx_t)1) {
            mat_mul(result, base, temp);
            mat_copy(temp, result);
        }
        exp >>= 1;
        if (!exp)
            break;
        mat_mul(base, base, temp);
        mat_copy(temp, base);
    }

    seq_t ans = (seq_t)0;
    for (size_t i = 0; i < B; ++i)
        ans += result[i][i];

    return ans;
}
