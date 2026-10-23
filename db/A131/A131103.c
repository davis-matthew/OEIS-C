/*
 * A131103  Rectangular array read by antidiagonals: a(n, k) is the number of
 *          ways to put k labeled objects into n labeled boxes so that there
 *          are no boxes with exactly one object (n, k >= 1).
 * https://oeis.org/A131103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = a(i,j) where i,j are found by reading the rectangular
 *            array a(i,j) of the number of ways to put j labeled objects into
 *            i labeled boxes with no box containing exactly one object. The
 *            array is read by antidiagonals, i decreasing within each
 *            antidiagonal
 * Range:     u8 n <= 22, u16 n <= 46, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *factorial_tbl = NULL;
static seq_t **T_tbl = NULL;
#define MAX_K 2000  /* sufficiently large for n up to ~2·10^6 */

/* initialise factorials and T table */
static void init_tables(void)
{
    /* factorials */
    factorial_tbl = (seq_t *)malloc((MAX_K + 1) * sizeof(seq_t));
    factorial_tbl[0] = (seq_t)1;
    for (idx_t i = 1; i <= MAX_K; ++i)
        factorial_tbl[i] = factorial_tbl[i-1] * (seq_t)i;

    /* 2‑D table T[k][j] of partitions of k into j blocks of size ≥2 */
    T_tbl = (seq_t**)malloc((MAX_K + 1) * sizeof(seq_t*));
    seq_t *data = (seq_t*)calloc((MAX_K + 1) * (MAX_K + 1), sizeof(seq_t));
    for (idx_t k = 0; k <= MAX_K; ++k)
        T_tbl[k] = data + k * (MAX_K + 1);

    T_tbl[0][0] = (seq_t)1;  /* base case */

    for (idx_t k = 2; k <= MAX_K; ++k)            /* k ≥ 2 */
    {
        idx_t jmax = k / 2;                        /* j cannot exceed floor(k/2) */
        for (idx_t j = 1; j <= jmax; ++j)
        {
            seq_t term1 = (seq_t)j * T_tbl[k-1][j];
            seq_t term2 = (seq_t)(k-1) * T_tbl[k-2][j-1];
            T_tbl[k][j] = term1 + term2;
        }
    }
}

/* compute a(n,k) using pre‑computed tables */
static seq_t compute_a_ik(idx_t nb, idx_t kj)
{
    if (kj == 0 || nb == 0) return (seq_t)0;

    idx_t max_b = kj / 2;              /* maximum number of non‑empty boxes */
    if (max_b > nb) max_b = nb;
    seq_t sum = (seq_t)0;

    for (idx_t b = 1; b <= max_b; ++b)
    {
        seq_t nPk = factorial_tbl[nb] / factorial_tbl[nb - b];
        seq_t term = T_tbl[kj][b] * nPk;
        sum += term;
    }
    return sum;
}

/* Main function requested by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of offset range */

    /* init tables on first use */
    if (!factorial_tbl) init_tables();

    /* determine i (boxes) and k (objects) from the antidiagonal index n */
    idx_t s = 2;  /* current sum i+j */
    while (((s - 1) * s) / 2 < n) ++s;   /* small loop, s ≤ ~1414 for n≤1e6 */

    idx_t tot_prev = ((s - 2) * (s - 1)) / 2;
    idx_t offset = n - tot_prev - (idx_t)1;  /* 0‑based offset on the diagonal */
    idx_t i = s - 1 - offset;                /* boxes (i) */
    idx_t k = s - i;                         /* objects (k) */

    if (i > MAX_K || k > MAX_K) return (seq_t)0;  /* safety */

    return compute_a_ik(i, k);
}
