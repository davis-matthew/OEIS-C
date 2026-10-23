/*
 * A125395  Number of base 32 circular n-digit numbers with adjacent digits
 *          differing by 5 or less.
 * https://oeis.org/A125395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = trace(A^n)
 * where A[i][j] = 1 if |i-j| <= 5 else 0.
 * a(0) = 1 (empty circular number)
 */
#include <stdint.h>

static void mat_mul(const seq_t a[32][32], const seq_t b[32][32], seq_t out[32][32])
{
    seq_t tmp[32][32];
    for (int i = 0; i < 32; ++i) {
        for (int j = 0; j < 32; ++j) {
            unsigned long long s = 0;
            for (int k = 0; k < 32; ++k) {
                s += (unsigned long long)a[i][k] * (unsigned long long)b[k][j];
            }
            tmp[i][j] = (seq_t)s;
        }
    }
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            out[i][j] = tmp[i][j];
}

seq_t a(idx_t n)
{
    if (n == 0ULL) return (seq_t)1;

    /* adjacency matrix A (base 32, difference <=5) */
    static seq_t base[32][32];
    static int base_init = 0;
    if (!base_init) {
        for (int i = 0; i < 32; ++i) {
            for (int j = 0; j < 32; ++j) {
                int d = i > j ? i - j : j - i;
                base[i][j] = (d <= 5) ? (seq_t)1 : (seq_t)0;
            }
        }
        base_init = 1;
    }

    /* fast exponentiation of the adjacency matrix */
    seq_t res[32][32];   /* identity */
    seq_t cur[32][32];   /* current power of A */

    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j) {
            res[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
            cur[i][j] = base[i][j];
        }

    idx_t exp = n;
    while (exp) {
        if (exp & 1ULL)
            mat_mul(res, cur, res);
        mat_mul(cur, cur, cur);
        exp >>= 1ULL;
    }

    /* trace of the result matrix */
    seq_t trace = 0;
    for (int i = 0; i < 32; ++i)
        trace += res[i][i];

    return trace;
}
