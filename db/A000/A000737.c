/*
 * A000737  Boustrophedon transform of natural numbers, cf. A000027.
 * https://oeis.org/A000737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Boustrophedon transform of b(i) = i+1: T(i,0) = i+1, T(i,j) =
 *            T(i,j-1) + T(i-1,i-j), a(n) = T(n,n); a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define A737_MAXN 12002

static seq_t a737_rowA[A737_MAXN];
static seq_t a737_rowB[A737_MAXN];
static seq_t a737_res[A737_MAXN];
static idx_t a737_cnt = 0;
static seq_t *a737_prev = a737_rowA;
static seq_t *a737_cur = a737_rowB;

seq_t a(idx_t n) {
    if (n >= A737_MAXN) return (seq_t)0;
    if (a737_cnt == 0) {
        a737_prev[0] = 1;
        a737_res[0] = 1;
        a737_cnt = 1;
    }
    while (a737_cnt <= n) {
        idx_t i = a737_cnt, j;
        seq_t *tmp;
        a737_cur[0] = (seq_t)i + 1;
        for (j = 1; j <= i; j++)
            a737_cur[j] = a737_cur[j - 1] + a737_prev[i - j];
        a737_res[i] = a737_cur[i];
        tmp = a737_prev;
        a737_prev = a737_cur;
        a737_cur = tmp;
        a737_cnt++;
    }
    return a737_res[n];
}
