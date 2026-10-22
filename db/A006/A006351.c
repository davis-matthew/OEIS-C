/*
 * A006351  Number of series-parallel networks with n labeled edges. Also
 *          called yoke-chains by Cayley and MacMahon.
 * https://oeis.org/A006351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   e.g.f. A(x) satisfies A' = (1+A)/(1-A), A(0) = 0, which gives
 *            a(m+1) = 2*a(m) + Sum_{k=1..m-1} binomial(m,k)*a(k)*a(m+1-k) for
 *            m >= 1, with a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *A6351_val = 0;   /* A6351_val[k] = a(k), valid for 1..have */
static seq_t *A6351_row = 0;   /* Pascal row number crow */
static idx_t A6351_cap = 0;
static idx_t A6351_have = 0;
static idx_t A6351_crow = 0;

static void A6351_ensure(idx_t need) {
    if (need <= A6351_cap) return;
    idx_t newcap = A6351_cap * 2;
    if (newcap < need) newcap = need;
    if (A6351_cap == 0) {
        A6351_val = alloc_zero(newcap);
        A6351_row = alloc_zero(newcap);
        A6351_row[0] = 1;
        A6351_crow = 0;
        A6351_val[1] = 1;
        A6351_have = 1;
    } else {
        A6351_val = grow_zero(A6351_val, A6351_cap, newcap);
        A6351_row = grow_zero(A6351_row, A6351_cap, newcap);
    }
    A6351_cap = newcap;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    A6351_ensure(n + 2);
    while (A6351_have < n) {
        idx_t m = A6351_have;
        /* advance Pascal row to row m */
        while (A6351_crow < m) {
            A6351_crow++;
            for (idx_t k = A6351_crow; k >= 1; k--)
                A6351_row[k] += A6351_row[k - 1];
        }
        seq_t s = A6351_val[m] * 2;
        for (idx_t k = 1; k + 1 <= m; k++) {
            seq_t t = A6351_row[k] * A6351_val[k];
            t *= A6351_val[m + 1 - k];
            s += t;
        }
        A6351_val[m + 1] = s;
        A6351_have = m + 1;
    }
    return A6351_val[n];
}
