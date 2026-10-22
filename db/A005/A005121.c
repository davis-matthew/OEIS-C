/*
 * A005121  Number of ultradissimilarity relations on an n-set.
 * https://oeis.org/A005121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{k=1..n-1} Stirling2(n, k)*a(k), a(1) = 1 [Lengyel]
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 22,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *S = NULL;     /* current row of Stirling2: S[j] = Stirling2(S_row, j) */
static idx_t S_cap = 0;
static idx_t S_row = 0;

static seq_t *A = NULL;     /* A[i] = a(i) for 1 <= i <= A_done */
static idx_t A_cap = 0;
static idx_t A_done = 0;

static void ensure_cap(idx_t len) {
    if (A_cap < len) {
        if (A == NULL) A = alloc_zero(len);
        else A = grow_zero(A, A_cap, len);
        A_cap = len;
    }
    if (S_cap < len) {
        if (S == NULL) {
            S = alloc_zero(len);
            S[0] = 1;
            S_row = 0;
        } else {
            S = grow_zero(S, S_cap, len);
        }
        S_cap = len;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n <= A_done) return A[n];

    ensure_cap(n + 1);

    while (A_done < n) {
        idx_t i = A_done + 1;
        while (S_row < i) {
            idx_t r = S_row + 1;
            for (idx_t j = r; j >= 1; j--) {
                S[j] = (seq_t)j * S[j] + S[j - 1];
            }
            S[0] = 0;
            S_row = r;
        }
        if (i == 1) {
            A[1] = 1;
        } else {
            seq_t sum = 0;
            for (idx_t k = 1; k < i; k++) {
                sum += S[k] * A[k];
            }
            A[i] = sum;
        }
        A_done = i;
    }
    return A[n];
}
