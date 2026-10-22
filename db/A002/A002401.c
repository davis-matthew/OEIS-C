/*
 * A002401  Coefficients for step-by-step integration.
 * https://oeis.org/A002401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm{1,2,...,n+1} * Sum_{k=0..n} |s(n,k)|/(k+1), where
 *            |s(n,k)| are the unsigned Stirling numbers of the first kind
 *            (equivalently a(n) = lcm{1..n+1} * Integral_{0..1}
 *            x(x+1)...(x+n-1) dx)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   a002401_ppbase
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a002401_ppbase.h"

#define A002401_MAXN 20002

static seq_t a002401_row[A002401_MAXN];
static idx_t a002401_rown = 0;
static bool a002401_init = false;

seq_t a(idx_t n) {
    idx_t i, j, k;
    seq_t L, sum;

    if (n + 1 >= A002401_MAXN) return 0;

    if (!a002401_init || a002401_rown > n) {
        for (j = 0; j < A002401_MAXN; j++) a002401_row[j] = 0;
        a002401_row[0] = 1;
        a002401_rown = 0;
        a002401_init = true;
    }
    /* advance unsigned Stirling row: c(i,j) = (i-1)*c(i-1,j) + c(i-1,j-1) */
    for (i = a002401_rown + 1; i <= n; i++) {
        for (j = i; j >= 1; j--) {
            a002401_row[j] = a002401_row[j] * (seq_t)(i - 1) + a002401_row[j - 1];
        }
        a002401_row[0] = 0;
        a002401_rown = i;
    }

    L = 1;
    for (i = 2; i <= n + 1; i++) {
        uint64_t p = a002401_ppbase((uint64_t)i);
        if (p > 1) L *= (seq_t)p;
    }

    sum = 0;
    for (k = 0; k <= n; k++) {
        sum += a002401_row[k] * (L / (seq_t)(k + 1));
    }
    return sum;
}
