/*
 * A106791  Sum of two consecutive squares of Lucas 4-step numbers (A073817).
 * https://oeis.org/A106791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A073817(n)^2 + A073817(n+1)^2 where A073817 is the Lucas
 *            4‑step sequence defined by L(0)=4, L(1)=1, L(2)=3, L(3)=7,
 *            L(n)=L(n-1)+L(n-2)+L(n-3)+L(n-4) for n≥4
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values of the Lucas 4‑step sequence. */
    seq_t L0 = (seq_t)4;
    seq_t L1 = (seq_t)1;
    seq_t L2 = (seq_t)3;
    seq_t L3 = (seq_t)7;

    /* Handle the first three indices explicitly. */
    if (n == 0) {
        return L0 * L0 + L1 * L1;          /* L0 and L1 */
    }
    if (n == 1) {
        return L1 * L1 + L2 * L2;          /* L1 and L2 */
    }
    if (n == 2) {
        return L2 * L2 + L3 * L3;          /* L2 and L3 */
    }

    /* For n ≥ 3 compute L(n) and L(n+1) iteratively. */
    idx_t i;
    for (i = 4; i <= n + 1; ++i) {
        seq_t next = L0 + L1 + L2 + L3;    /* L(i) */
        L0 = L1;
        L1 = L2;
        L2 = L3;
        L3 = next;
    }

    /* After the loop: L2 = L(n),  L3 = L(n+1). */
    return L2 * L2 + L3 * L3;
}
