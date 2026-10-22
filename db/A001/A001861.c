/*
 * A001861  Expansion of e.g.f. exp(2*(exp(x) - 1)).
 * https://oeis.org/A001861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=0..n} 2^k*Stirling2(n, k), with Stirling2(i, k) =
 *            k*Stirling2(i-1, k) + Stirling2(i-1, k-1), Stirling2(0, 0) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define A001861_MAXN 20001

static seq_t a001861_S[A001861_MAXN + 1];
static seq_t a001861_A[A001861_MAXN + 1];
static idx_t a001861_done = 0;
static bool a001861_init = false;

seq_t a(idx_t n) {
    if (!a001861_init) {
        a001861_S[0] = 1;
        a001861_A[0] = 1;
        a001861_done = 0;
        a001861_init = true;
    }
    if (n > A001861_MAXN) return 0;
    while (a001861_done < n) {
        idx_t i = a001861_done + 1;
        seq_t r = 0;
        a001861_S[i] = 0;
        for (idx_t k = i; k >= 1; k--) {
            a001861_S[k] = a001861_S[k] * k + a001861_S[k - 1];
            r = (r + a001861_S[k]) * 2;
        }
        a001861_S[0] = 0;
        a001861_A[i] = r;
        a001861_done = i;
    }
    return a001861_A[n];
}
