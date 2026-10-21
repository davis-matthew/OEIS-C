/*
 * A213785  a(n) = Sum(P(i)*P(j), 1<=i<j<=n), where P(k) is the k-th Pell
 *          number A000129(k).
 * https://oeis.org/A213785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( (∑_{i=1}^n P_i)^2 – ∑_{i=1}^n P_i^2 ) / 2 where P_0 =
 *            0, P_1 = 1, P_k = 2·P_{k-1} + P_{k-2} for k ≥ 2 a(0) = 0, a(1) =
 *            0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* static tables holding Pell numbers and their prefix sums */
static seq_t *P   = NULL;  /* P[0..n]                     */
static seq_t *S1  = NULL;  /* S1[k] = ∑_{i=0}^k P[i]      */
static seq_t *S2  = NULL;  /* S2[k] = ∑_{i=0}^k P[i]^2   */
static size_t  cap   = 0;  /* current allocated size     */
static idx_t   idx_max = (idx_t)(-1); /* largest index computed */

/* ensure the tables can hold indices up to n */
static void ensure_capacity(idx_t n)
{
    if (n < (idx_t)cap)
        return;                       /* already large enough */

    size_t newcap = cap ? cap * 2 : 4;
    while (newcap <= (size_t)n)
        newcap <<= 1;                  /* double until >= n+1 */

    seq_t *newP  = (seq_t *)malloc(newcap * sizeof(seq_t));
    seq_t *newS1 = (seq_t *)malloc(newcap * sizeof(seq_t));
    seq_t *newS2 = (seq_t *)malloc(newcap * sizeof(seq_t));
    if (!newP || !newS1 || !newS2) {
        /* allocation failed – abort, as the sequence cannot be computed */
        exit(EXIT_FAILURE);
    }

    if (P)  memcpy(newP,  P,  (idx_max + 1) * sizeof(seq_t));
    if (S1) memcpy(newS1, S1, (idx_max + 1) * sizeof(seq_t));
    if (S2) memcpy(newS2, S2, (idx_max + 1) * sizeof(seq_t));

    free(P);  free(S1);  free(S2);
    P = newP; S1 = newS1; S2 = newS2;
    cap = newcap;
}

/* compute a(n) for any n ≥ 0, independent of order of calls */
seq_t a(idx_t n)
{
    /* trivial cases – a(0) = a(1) = 0 */
    if (n <= 1)
        return (seq_t)0;

    /* compute up to n if not yet done */
    if ((idx_t)cap == 0 || n >= (idx_t)cap)
        ensure_capacity(n);

    /* initialise k = 0 entry if the tables are empty */
    if (idx_max == (idx_t)(-1)) {
        P[0]   = (seq_t)0;
        S1[0]  = (seq_t)0;
        S2[0]  = (seq_t)0;
        idx_max = 0;
    }

    /* extend tables from idx_max+1 up to n inclusive */
    for (idx_t k = idx_max + 1; k <= n; ++k)
    {
        if (k == 1) {          /* P[1] is already set by init or extended later */
            P[1]  = (seq_t)1;
            S1[1] = P[1];
            S2[1] = P[1] * P[1];
        } else {               /* k ≥ 2 */
            P[k]  = (seq_t)2 * P[k-1] + P[k-2];
            S1[k] = S1[k-1] + P[k];
            S2[k] = S2[k-1] + P[k] * P[k];
        }
    }
    idx_max = n;

    /* a(n) = (S1[n]^2 – S2[n]) / 2 */
    seq_t sq = S1[n] * S1[n];
    seq_t tmp = sq - S2[n];
    return tmp / (seq_t)2;
}
