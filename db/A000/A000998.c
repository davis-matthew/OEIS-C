/*
 * A000998  From a differential equation.
 * https://oeis.org/A000998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   with b = {0,0,1,0,0, a(0), a(1), ...}, i.e. a(n) = b(n+5),
 *            b(0)=b(1)=0, b(2)=1, b(n+3) = Sum_{i=0..n} binomial(n,i)*b(i)
 *            (sequence shifts 3 places under binomial transform); computed by
 *            an addition-only difference table T(m,j) = T(m-1,j) +
 *            T(m-1,j+1), T(0,j) = b(j), b(m+3) = T(m,0)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 31, u128 n <= 53,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *B = 0;      /* b values */
static seq_t *D = 0;      /* last computed anti-diagonal of T */
static idx_t cap = 0;     /* capacity of D; B has capacity cap + 3 */
static idx_t s_done = 0;  /* diagonals 0..s_done-1 computed */

static void ensure_cap(idx_t need) {
    idx_t newcap, i;
    seq_t *nB, *nD;
    if (need <= cap) return;
    newcap = cap * 2;
    if (newcap < need) newcap = need;
    if (newcap < 16) newcap = 16;
    nB = alloc_zero(newcap + 3);
    nD = alloc_zero(newcap);
    if (cap == 0) {
        nB[0] = 0; nB[1] = 0; nB[2] = 1;
    } else {
        for (i = 0; i < cap + 3; i++) nB[i] = B[i];
        for (i = 0; i < cap; i++) nD[i] = D[i];
        release(B, cap + 3);
        release(D, cap);
    }
    B = nB;
    D = nD;
    cap = newcap;
}

seq_t a(idx_t n) {
    idx_t N = n + 3;  /* need diagonals 0..n+2 */
    ensure_cap(N + 1);
    while (s_done < N) {
        idx_t s = s_done, i;
        seq_t prev = D[0];
        D[0] = B[s];
        for (i = 1; i <= s; i++) {
            seq_t old = (i < s) ? D[i] : (seq_t)0;
            D[i] = D[i - 1] + prev;
            prev = old;
        }
        B[s + 3] = D[s];
        s_done++;
    }
    return B[n + 5];
}
