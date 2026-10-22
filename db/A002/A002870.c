/*
 * A002870  Largest Stirling numbers of second kind: a(n) = max_{k=1..n}
 *          S2(n,k).
 * https://oeis.org/A002870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = max_{k=1..n} S2(n,k), where S2(n,k) = k*S2(n-1,k) +
 *            S2(n-1,k-1), S2(0,0) = 1, S2(n,0) = 0 for n > 0, S2(0,k) = 0 for
 *            k > 0. Rows of the Stirling triangle are built in place and
 *            memoized in static storage
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A002870_CAP 16384

static seq_t a002870_row[A002870_CAP + 1];
static seq_t a002870_best[A002870_CAP + 1];
static idx_t a002870_done = 0;

seq_t a(idx_t n)
{
    if (n == 0 || n > A002870_CAP) return 0;

    if (a002870_done == 0) {
        a002870_row[0] = 0;
        a002870_row[1] = 1;
        a002870_best[1] = 1;
        a002870_done = 1;
    }

    while (a002870_done < n) {
        idx_t i = a002870_done + 1;
        idx_t k;
        seq_t mx = 0;
        a002870_row[i] = 0;
        for (k = i; k >= 1; k--) {
            a002870_row[k] = (seq_t)k * a002870_row[k] + a002870_row[k - 1];
            if (a002870_row[k] > mx) mx = a002870_row[k];
        }
        a002870_best[i] = mx;
        a002870_done = i;
    }

    return a002870_best[n];
}
