/*
 * A001758  Number of quasi-alternating permutations of length n.
 * https://oeis.org/A001758
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*A000111(n+1) - 4*A000111(n), from 2*u' = 1 + u^2 with u
 *            = tan(x)+sec(x); A000111 (Euler zigzag numbers) computed by the
 *            Seidel-Entringer triangle: E(0,0) = 1, E(k,0) = 0, E(k,j) =
 *            E(k,j-1) + E(k-1,k-j), A000111(k) = E(k,k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A1758_MAX 20005

static seq_t a1758_E[A1758_MAX];
static seq_t a1758_rowA[A1758_MAX];
static seq_t a1758_rowB[A1758_MAX];
static seq_t *a1758_cur;
static seq_t *a1758_nxt;
static idx_t a1758_done;
static int a1758_init = 0;

seq_t a(idx_t n)
{
    idx_t need, k, j;
    if (n < 2) return 0;
    need = n + 1;
    if (need >= A1758_MAX) return 0;
    if (!a1758_init) {
        a1758_rowA[0] = 1;
        a1758_E[0] = 1;
        a1758_cur = a1758_rowA;
        a1758_nxt = a1758_rowB;
        a1758_done = 0;
        a1758_init = 1;
    }
    while (a1758_done < need) {
        seq_t *tmp;
        k = a1758_done + 1;
        a1758_nxt[0] = 0;
        for (j = 1; j <= k; j++)
            a1758_nxt[j] = a1758_nxt[j - 1] + a1758_cur[k - j];
        a1758_E[k] = a1758_nxt[k];
        tmp = a1758_cur;
        a1758_cur = a1758_nxt;
        a1758_nxt = tmp;
        a1758_done = k;
    }
    return a1758_E[n + 1] * 2 - a1758_E[n] * 4;
}
