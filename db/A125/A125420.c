/*
 * A125420  Number of base 32 circular n-digit numbers with adjacent digits
 *          differing by 6 or less.
 * https://oeis.org/A125420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) for n≥1; a(0)=1. A is a 32×32 adjacency matrix
 *            with A[i][j] = 1 if |i-j|≤6 (otherwise 0)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* See OEIS A125420 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define SZ 32

/* forward declaration of seq_t, idx_t as per build */
typedef idx_t idx_t;
typedef seq_t seq_t;

/* compile‑time helper for 128‑bit intermediate */
#ifdef __SIZEOF_INT128__
typedef unsigned __int128 u128;
#else
/* fallback: use long double for intermediate (only for very large mult) */
#endif

/* local static storage: adjacency matrix and its powers */
static seq_t A_mat[SZ][SZ];
static seq_t powers[15][SZ][SZ];   /* 2^0 .. 2^14 (covers n up to 16384) */
static bool initialized = false;

/* Initialize adjacency matrix and pre‑compute powers */
static void init_matrix(void) {
    if (initialized) return;
    /* build A_mat */
    for (idx_t i = 0; i < SZ; ++i) {
        for (idx_t j = 0; j < SZ; ++j) {
            if ((i > j ? i - j : j - i) <= 6)
                A_mat[i][j] = (seq_t)1;
            else
                A_mat[i][j] = (seq_t)0;
        }
    }
    /* powers[0] = A_mat */
    for (idx_t i = 0; i < SZ; ++i)
        for (idx_t j = 0; j < SZ; ++j)
            powers[0][i][j] = A_mat[i][j];

    /* matrix multiplication (seq_t) with 128‑bit intermediate */
    static seq_t temp[SZ][SZ];
    for (idx_t k = 1; k < 15; ++k) {
        for (idx_t i = 0; i < SZ; ++i) {
            for (idx_t j = 0; j < SZ; ++j) {
                u128 sum = 0;
                for (idx_t t = 0; t < SZ; ++t) {
                    sum += (u128)powers[k-1][i][t] *
                           (u128)powers[k-1][t][j];
                }
                temp[i][j] = (seq_t)sum;
            }
        }
        for (idx_t i = 0; i < SZ; ++i)
            for (idx_t j = 0; j < SZ; ++j)
                powers[k][i][j] = temp[i][j];
    }
    initialized = true;
}

/* matrix multiplication: R = X * Y */
static void mul_mat(const seq_t X[SZ][SZ], const seq_t Y[SZ][SZ],
                    seq_t R[SZ][SZ]) {
    static seq_t temp[SZ][SZ];
    for (idx_t i = 0; i < SZ; ++i) {
        for (idx_t j = 0; j < SZ; ++j) {
            u128 sum = 0;
            for (idx_t t = 0; t < SZ; ++t) {
                sum += (u128)X[i][t] * (u128)Y[t][j];
            }
            temp[i][j] = (seq_t)sum;
        }
    }
    for (idx_t i = 0; i < SZ; ++i)
        for (idx_t j = 0; j < SZ; ++j)
            R[i][j] = temp[i][j];
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)1;   /* special case */

    init_matrix();                        /* ensure matrices ready */

    /* result = I (identity) */
    static seq_t res[SZ][SZ];
    for (idx_t i = 0; i < SZ; ++i)
        for (idx_t j = 0; j < SZ; ++j)
            res[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    idx_t bit = 0;
    idx_t mask = (idx_t)1;
    while (mask <= n) {
        if (n & mask) {
            mul_mat(res, powers[bit], res);   /* res = res * powers[bit] */
        }
        ++bit;
        mask <<= 1;
    }

    /* trace of res */
    u128 tr = 0;
    for (idx_t i = 0; i < SZ; ++i)
        tr += (u128)res[i][i];
    return (seq_t)tr;
}
