/*
 * A125375  Number of base 12 circular n-digit numbers with adjacent digits
 *          differing by 5 or less.
 * https://oeis.org/A125375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(Aⁿ), where A[i][j] = 1 if |i−j| ≤ 5, else 0 * (A is
 *            a 12 × 12 adjacency matrix). * a(0) = 1, a(n) counts circular
 *            base‑12 n‑digit numbers whose * consecutive digits differ by at
 *            most 5 (including the last‑to‑first pair)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define DIGITS 12

/* Global adjacency matrix, initialised once. */
static seq_t adj[DIGITS][DIGITS];
static bool adj_initialised = false;

static void init_adj(void)
{
    if (adj_initialised)
        return;
    for (int i = 0; i < DIGITS; ++i) {
        for (int j = 0; j < DIGITS; ++j) {
            int d = i - j;
            if (d < 0) d = -d;
            adj[i][j] = (d <= 5) ? 1 : 0;
        }
    }
    adj_initialised = true;
}

/* Matrix multiplication: out = M1 × M2 (both DIGITS×DIGITS). */
static void mat_mul(seq_t out[DIGITS][DIGITS],
                    const seq_t M1[DIGITS][DIGITS],
                    const seq_t M2[DIGITS][DIGITS])
{
    for (int i = 0; i < DIGITS; ++i) {
        for (int j = 0; j < DIGITS; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < DIGITS; ++k) {
                sum += M1[i][k] * M2[k][j];
            }
            out[i][j] = sum;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return 1;                /* empty string count */

    init_adj();

    /* result matrix starts as the identity. */
    seq_t res[DIGITS][DIGITS] = {0};
    seq_t tmp[DIGITS][DIGITS];
    for (int i = 0; i < DIGITS; ++i)
        res[i][i] = 1;

    /* base matrix is the adjacency matrix. */
    seq_t base[DIGITS][DIGITS];
    for (int i = 0; i < DIGITS; ++i)
        for (int j = 0; j < DIGITS; ++j)
            base[i][j] = adj[i][j];

    /* binary exponentiation of the matrix. */
    idx_t exp = n;
    while (exp > (idx_t)0) {
        if (exp & (idx_t)1) {
            mat_mul(tmp, res, base);
            for (int i = 0; i < DIGITS; ++i)
                for (int j = 0; j < DIGITS; ++j)
                    res[i][j] = tmp[i][j];
        }
        mat_mul(tmp, base, base);
        for (int i = 0; i < DIGITS; ++i)
            for (int j = 0; j < DIGITS; ++j)
                base[i][j] = tmp[i][j];
        exp >>= (idx_t)1;
    }

    /* trace of res gives the required count. */
    seq_t trace = 0;
    for (int i = 0; i < DIGITS; ++i)
        trace += res[i][i];

    return trace;
}
