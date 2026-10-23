/*
 * A125467  Number of base 32 circular n-digit numbers with adjacent digits
 *          differing by 8 or less.
 * https://oeis.org/A125467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A125467
   Formula: a(0)=1, a(n)=trace(Adj^n) for n≥1
   where Adj is a 32×32 adjacency matrix with
   Adj[i][j]=1 iff |i−j|≤8 (no wrap around), else 0.  */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>          /* for bool */

#define DIGITS 32
#define MAX_DIFF 8

/* adjacency matrix (built once) */
static seq_t adj_mat[DIGITS][DIGITS];
static bool adj_ready = false;
static void build_adj(void)
{
    if (adj_ready) return;
    for (int i = 0; i < DIGITS; ++i)
        for (int j = 0; j < DIGITS; ++j)
            adj_mat[i][j] = ((i - j >= -MAX_DIFF && i - j <= MAX_DIFF) ? 1 : 0);
    adj_ready = true;
}

/* matrix helpers ---------------------------------------------- */
static void mat_identity(seq_t M[DIGITS][DIGITS])
{
    for (int i = 0; i < DIGITS; ++i)
        for (int j = 0; j < DIGITS; ++j)
            M[i][j] = (i == j) ? 1 : 0;
}

static void mat_copy(const seq_t src[DIGITS][DIGITS], seq_t dst[DIGITS][DIGITS])
{
    for (int i = 0; i < DIGITS; ++i)
        for (int j = 0; j < DIGITS; ++j)
            dst[i][j] = src[i][j];
}

static void mat_mul(const seq_t A[DIGITS][DIGITS], const seq_t B[DIGITS][DIGITS],
                    seq_t R[DIGITS][DIGITS])
{
    for (int i = 0; i < DIGITS; ++i)
    {
        for (int j = 0; j < DIGITS; ++j)
        {
            seq_t sum = 0;
            for (int k = 0; k < DIGITS; ++k)
                sum += A[i][k] * B[k][j];
            R[i][j] = sum;
        }
    }
}

/* fast exponentiation by squaring -------------------------------- */
static void mat_pow(const seq_t base[DIGITS][DIGITS], seq_t res[DIGITS][DIGITS],
                    idx_t exp)
{
    seq_t power[DIGITS][DIGITS];
    seq_t tmp[DIGITS][DIGITS];

    mat_identity(res);
    mat_copy(base, power);

    while (exp > 0)
    {
        if (exp & 1ULL)
        {
            mat_mul(res, power, tmp);
            mat_copy(tmp, res);
        }
        mat_mul(power, power, tmp);
        mat_copy(tmp, power);
        exp >>= 1;
    }
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0) return (seq_t)1;

    build_adj();

    seq_t P[DIGITS][DIGITS];
    mat_pow(adj_mat, P, n);

    /* trace */
    seq_t trace = 0;
    for (int i = 0; i < DIGITS; ++i)
        trace += P[i][i];
    return trace;
}
