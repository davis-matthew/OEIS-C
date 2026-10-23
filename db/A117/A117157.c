/*
 * A117157  a(1)=a(2)=1; a(n) = a(n-1)*a(n-2) + a(n-3) + a(n-4) + ... + a(1)
 *          for n>2.
 * https://oeis.org/A117157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) * a(n-2) + a(n-3) + a(n-4) + ... + a(1) Initial
 *            values: a(1) = a(2) = 1
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 10, u64 n <= 12, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *A = NULL;      /* array of a(n) values (0‑based)         */
static seq_t *P = NULL;      /* prefix sums: P[i] = a(1)+...+a(i+1)    */
static size_t sz = 0;        /* number of elements stored             */
static size_t cap = 0;       /* current allocation capacity           */

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined, but guard against underflow */

    /* convert to 0‑based index */
    size_t idx = (size_t)(n - 1);

    /* ensure base cases are stored */
    if (sz == 0) {
        cap = 4;
        A = (seq_t *)realloc(A, cap * sizeof(seq_t));
        P = (seq_t *)realloc(P, cap * sizeof(seq_t));
        if (!A || !P) return 0; /* allocation failure */

        A[0] = 1; P[0] = 1;          /* a(1) = 1 */
        A[1] = 1; P[1] = 2;          /* a(2) = 1, prefix = 1+1 */
        sz = 2;
    }

    /* extend sequence up to required index */
    while (sz <= idx) {
        /* product of previous two terms */
        seq_t prod = A[sz - 1] * A[sz - 2];

        /* sum of all terms except last two: prefix up to sz-3 */
        seq_t sumPrev = 0;
        if (sz >= 3) sumPrev = P[sz - 3];

        /* new term */
        seq_t newv = prod + sumPrev;

        /* ensure capacity */
        if (sz == cap) {
            size_t newcap = cap * 2;
            seq_t *tmpA = (seq_t *)realloc(A, newcap * sizeof(seq_t));
            seq_t *tmpP = (seq_t *)realloc(P, newcap * sizeof(seq_t));
            if (!tmpA || !tmpP) return 0; /* allocation failure */
            A = tmpA; P = tmpP; cap = newcap;
        }

        /* store new term and update prefix sum */
        A[sz] = newv;
        P[sz] = P[sz - 1] + newv;
        sz++;
    }

    return A[idx];
}
