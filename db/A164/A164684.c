/*
 * A164684  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A164684 – Number of reduced words of length n in the
 * Coxeter group on 40 generators with relations S_i^2 = (S_iS_j)^7 = I.
 *
 * Recurrence:  a(n) = 38*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6))
 *                     - 741*a(n-7)  for n ≥ 8
 *
 * Initial values:
 *   a(0)=1      a(1)=40      a(2)=1560
 *   a(3)=60840  a(4)=2372760 a(5)=92537640
 *   a(6)=3608967960
 *   a(7)=140749749660
 *
 * The function computes a(n) using only this recurrence and does
 * not rely on a pre‑computed table of more than the first eight terms.
 */

#include <stddef.h>   /* for size_t, if needed */

seq_t a(idx_t n)
{
    /* first eight terms, needed to start the recurrence */
    static const seq_t init[8] = {
        (seq_t)1,
        (seq_t)40,
        (seq_t)1560,
        (seq_t)60840,
        (seq_t)2372760,
        (seq_t)92537640,
        (seq_t)3608967960ULL,
        (seq_t)140749749660ULL
    };

    if (n < 8)   /* return the known first eight terms directly */
        return init[n];

    /* array holding the six previous terms (a(n-1)..a(n-6))
       and the seventh previous term a(n-7) in index 6 */
    seq_t prev[7] = {
        init[7], /* a(n-1) when n==8   -> a(7) */
        init[6], /* a(n-2)           -> a(6) */
        init[5], /* a(n-3)           -> a(5) */
        init[4], /* a(n-4)           -> a(4) */
        init[3], /* a(n-5)           -> a(3) */
        init[2], /* a(n-6)           -> a(2) */
        init[1]  /* a(n-7)           -> a(1) */
    };

    seq_t result = init[7]; /* value of a(n-1) for the first loop */

    /* iterate from n==8 up to the requested n */
    for (idx_t i = 8; i <= n; ++i) {
        seq_t sum = prev[0] + prev[1] + prev[2] + prev[3] + prev[4] + prev[5];
        seq_t new_val = (seq_t)38 * sum - (seq_t)741 * prev[6];

        /* shift previous terms: a(n-1) becomes a(n-2), etc. */
        prev[6] = prev[5];
        prev[5] = prev[4];
        prev[4] = prev[3];
        prev[3] = prev[2];
        prev[2] = prev[1];
        prev[1] = prev[0];
        prev[0] = new_val;

        result = new_val;
    }

    return result;
}
