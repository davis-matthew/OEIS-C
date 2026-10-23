/*
 * A169050  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169050
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

/*  OEIS A169050: Number of reduced words of length n in the Coxeter group on
    37 generators with relations S_i^2 = (S_i S_j)^24 = I.
    This implementation uses the following recurrence derived from
    the pattern‑avoiding description of reduced words.

    Let S(n) be the number of words of length n ending with a single
    letter (i.e. the last letter has no partner).
    For l = 2 … 23 let P(n,l) be the number of words of length n that
    end with an alternating run of length l between two distinct letters.

    Base (n=1):
        S(1) = 37 ,   P(1,l) = 0   for all l.
    For n ≥ 2:
        S(n)   = 35 * Σ_{l=2}^{23} P(n-1,l)
        P(n,2) = 36 * S(n-1)
        P(n,l) = P(n-1,l-1)   for l = 3 … 23   (extension of an existing run)
                  (runs of length 24 are forbidden, so the l=23 runs
                   cannot be extended; they contribute only to S(n)).

    The desired a(n) is the sum of all valid endings:
        a(n) = S(n) + Σ_{l=2}^{23} P(n,l)

    This recurrence is O(n) and works for arbitrarily large n
    (limited only by the size of seq_t).                                         */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 0‑offset: a(0)=1  */
    if (n == 0)
        return (seq_t)1;

    /* n==1 */
    if (n == 1)
        return (seq_t)37;

    /* states for current length */
    seq_t S = (seq_t)37;          /* S(1) */
    seq_t P[24] = {0};            /* indices 2..23 used */

    /* iterate from length 2 up to n */
    for (idx_t len = 2; len <= n; ++len)
    {
        /* compute sum of all P[n-1][l] */
        seq_t sumP = 0;
        for (int l = 2; l <= 23; ++l)
            sumP += P[l];

        /* new single‑letter endings */
        seq_t S_new = sumP * 35;

        /* new alternating runs */
        seq_t newP[24] = {0};
        newP[2] = S * 36;              /* extend singles to length 2 */
        for (int l = 3; l <= 23; ++l)
            newP[l] = P[l - 1];        /* extend existing runs */

        /* update states */
        S = S_new;
        for (int l = 2; l <= 23; ++l)
            P[l] = newP[l];
    }

    /* total number of words of length n */
    seq_t total = S;
    for (int l = 2; l <= 23; ++l)
        total += P[l];

    return total;
}
