/*
 * A015097  Carlitz-Riordan q-Catalan numbers (recurrence version) for q=-2.
 * https://oeis.org/A015097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 Recurrence: for n >= 0, a(n+1) = sum_{i = 0..n} (-2)^i
 *            * a(i) * a(n-i) (Carlitz‑Riordan q‑Catalan numbers for q = -2)
 *            The function returns a(n) for any n >= 0 with memoised values
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 8, i64 n <= 11, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static buffers for memoisation */
    static seq_t *A    = NULL;   /* Holds a[0..] values   */
    static idx_t  Amax = 0;      /* Highest index computed so far (>= 0). */
    static size_t Acap = 0;      /* Allocated capacity of A. */

    static seq_t *P    = NULL;   /* Holds (-2)^i values for i >= 0 */
    static idx_t  Pmax = 0;      /* Highest i computed so far. */
    static size_t Pcap = 0;      /* Allocated capacity of P. */

    /* Initialise buffers on first call. */
    if (A == NULL)
    {
        Acap = (n + 1 > 64) ? n + 1 : 64;
        A    = (seq_t *)malloc(Acap * sizeof(seq_t));
        A[0] = (seq_t)1;            /* a(0) = 1 */
        Amax = 0;
    }
    else if (n + 1 > Acap)          /* Grow A if necessary. */
    {
        size_t newcap = Acap ? Acap * 2 : 64;
        while (newcap < (size_t)(n + 1))
            newcap *= 2;
        A = (seq_t *)realloc(A, newcap * sizeof(seq_t));
        Acap = newcap;
    }

    if (P == NULL)
    {
        Pcap = (n + 1 > 64) ? n + 1 : 64;
        P    = (seq_t *)malloc(Pcap * sizeof(seq_t));
        P[0] = (seq_t)1;            /* (-2)^0 = 1 */
        Pmax = 0;
    }
    else if (n + 1 > Pcap)          /* Grow P if necessary. */
    {
        size_t newcap = Pcap ? Pcap * 2 : 64;
        while (newcap < (size_t)(n + 1))
            newcap *= 2;
        P = (seq_t *)realloc(P, newcap * sizeof(seq_t));
        Pcap = newcap;
    }

    /* Pre‑compute powers of -2 up to required index. */
    while (Pmax < n)
    {
        /* Compute (-2)^(Pmax+1) from the previous value. */
        P[Pmax + 1] = (seq_t)-2 * P[Pmax];
        ++Pmax;
    }

    /* If the requested term is already known, return it. */
    if (n <= Amax)
        return A[n];

    /* Compute missing terms incrementally. */
    idx_t i;
    for (i = Amax + 1; i <= n; ++i)
    {
        seq_t sum = 0;
        idx_t j;
        for (j = 0; j <= i - 1; ++j)
        {
            /* term = (-2)^j * a(j) * a(i-1-j) */
            sum += P[j] * A[j] * A[i - 1 - j];
        }
        A[i] = sum;
    }
    Amax = n;   /* Update the maximum computed index. */

    return A[n];
}
