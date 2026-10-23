/*
 * A133037  a(n) = A000931(n)^2.
 * https://oeis.org/A133037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Padovan(n)^2 Padovan recurrence: P(0)=1,P(1)=0,P(2)=0,
 *            and for n≥3, P(n)=P(n-2)+P(n-3). Then a(n)=P(n)^2. Initial
 *            values: a(0)=1, a(1)=0, a(2)=0
 * Range:     u8 n <= 15, u16 n <= 25, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *P = NULL;      /* Padovan numbers        */
    static seq_t *A = NULL;      /* a(n) = P(n)^2          */
    static idx_t len = 0;        /* next index to compute  */
    static idx_t cap = 0;        /* allocated capacity     */

    /* Lazy initialisation of seeds */
    if (P == NULL) {
        cap = 16;
        P = (seq_t*)malloc(cap * sizeof(seq_t));
        A = (seq_t*)malloc(cap * sizeof(seq_t));

        P[0] = 1;  A[0] = 1;   /* P(0)=1, a(0)=1^2 */
        P[1] = 0;  A[1] = 0;   /* P(1)=0, a(1)=0^2 */
        P[2] = 0;  A[2] = 0;   /* P(2)=0, a(2)=0^2 */

        len = 3;          /* we have computed indices 0..2 */
    }

    /* Extend cache if n is beyond what we have stored */
    if (n >= len) {
        if (n >= cap) {
            idx_t new_cap = cap ? cap * 2 : 16;
            while (new_cap <= n) new_cap *= 2;
            P = (seq_t*)realloc(P, new_cap * sizeof(seq_t));
            A = (seq_t*)realloc(A, new_cap * sizeof(seq_t));
            cap = new_cap;
        }

        for (idx_t i = len; i <= n; ++i) {
            seq_t val = P[i - 2] + P[i - 3];        /* Padovan recurrence */
            P[i] = val;
            A[i] = val * val;                       /* square   */
        }
        len = n + 1;
    }

    return A[n];
}
