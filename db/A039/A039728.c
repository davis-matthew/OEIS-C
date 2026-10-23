/*
 * A039728  (Even numbers in Hofstadter Q-sequence)/2.
 * https://oeis.org/A039728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (even terms of Hofstadter Q-sequence)/2. Hofstadter Q:
 *            Q(1)=Q(2)=1; for n>2, Q(n)=Q(n-Q(n-1))+Q(n-Q(n-2)). a(n) records
 *            Q(k)/2 for each occurrence of an even Q(k)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Forward declarations of the helper functions used only for memory allocation */
static void ensure_cap(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t newcap = (*cap) ? (*cap) * 2 : 16;
        while (newcap < need)
            newcap *= 2;
        *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

/* The main sequence function */
seq_t a(idx_t n)
{
    static seq_t *Q = NULL;          /* array for Hofstadter Q values */
    static size_t Qcap = 0, Qlen = 0;
    static seq_t *E = NULL;          /* array for even Q/2 values */
    static size_t Ecap = 0, Elen = 0;

    if (n == 0) return 0;  /* safety, though n>=1 as per problem statement */

    /* Return cached value if already computed */
    if (Elen >= n)
        return E[n - 1];

    /* Initialise Q with Q(1)=Q(2)=1 if it's the first call */
    if (Q == NULL) {
        ensure_cap(&Q, &Qcap, 2);
        Q[0] = 1;
        Q[1] = 1;
        Qlen = 2;
    }

    /* Grow even cache if needed */
    ensure_cap(&E, &Ecap, n); /* at least up to requested n */

    /* Compute Q values until we have gathered enough even terms */
    while (Elen < (size_t)n) {
        size_t i = Qlen;              /* index of the new term (zero‑based) */
        ensure_cap(&Q, &Qcap, i + 1); /* ensure room for Q[i] */

        /* Indices needed for the recurrence (zero‑based) */
        idx_t idx1 = i - (idx_t)Q[i - 1];   /* i - Q(i)  */
        idx_t idx2 = i - (idx_t)Q[i - 2];   /* i - Q(i-1) */

        seq_t val = Q[(size_t)idx1] + Q[(size_t)idx2];
        Q[i] = val;
        ++Qlen;

        /* If Q(i) is even, store half of it */
        if ((val & 1) == 0) {
            ensure_cap(&E, &Ecap, Elen + 1);
            E[Elen++] = val / 2;
        }
    }

    return E[n - 1];
}
