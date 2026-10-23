/*
 * A036600  Number of binary rooted trees with n nodes and height exactly 11.
 * https://oeis.org/A036600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = # binary rooted trees with n nodes and height exactly 11
 *            * Dynamic programming: a(n) = T[11][n] - T[10][n] * where
 *            T[h][n] is the number of trees with n nodes and height ≤ h. *
 *            Initial values: a(0)=0, a(1)=0, …, a(10)=0, a(11)=1, …
 * Range:     u8 n <= 14, u16 n <= 19, u32 n <= 32, u64 n <= 62, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *T_table[12];
static bool T_initialized = false;
static const idx_t MAX_N = (idx_t)((1ULL << 11) - 1); /* 2047 */

/* Build DP tables for heights 0..11 up to MAX_N nodes */
static void build_tables(void)
{
    if (T_initialized)
        return;

    for (int h = 0; h <= 11; ++h) {
        T_table[h] = (seq_t *)calloc((size_t)(MAX_N + 1), sizeof(seq_t));
        if (!T_table[h]) {
            exit(EXIT_FAILURE);
        }
    }

    /* Height 0: only the empty tree */
    T_table[0][0] = (seq_t)1;
    for (idx_t n = 1; n <= MAX_N; ++n)
        T_table[0][n] = (seq_t)0;

    /* Heights 1..11 */
    for (int h = 1; h <= 11; ++h) {
        seq_t *prev = T_table[h - 1];
        seq_t *cur  = T_table[h];

        cur[0] = (seq_t)1; /* empty tree */
        for (idx_t n = 1; n <= MAX_N; ++n) {
            seq_t sum = (seq_t)0;
            idx_t limit = (n - 1) / 2; /* i ≤ j */
            for (idx_t i = 0; i <= limit; ++i) {
                idx_t j = n - 1 - i;
                if (i < j) {
                    sum += prev[i] * prev[j];
                } else { /* i == j */
                    sum += prev[i] * (prev[i] + (seq_t)1) / (seq_t)2;
                }
            }
            cur[n] = sum;
        }
    }

    T_initialized = true;
}

seq_t a(idx_t n)
{
    build_tables();

    if (n > MAX_N)
        return (seq_t)0;

    return T_table[11][n] - T_table[10][n];
}
