/*
 * A106512  Array read by antidiagonals: a(n,k) = number of k-colorings of a
 *          circle of n nodes (n >= 1, k >= 1).
 * https://oeis.org/A106512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 72, u32 n <= 181, u64 n <= 452,
 *            u128 n <= 1159, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A106512.
   The array a(n,k) is defined by
        a(n,k) = (k-1)^n + (-1)^n *(k-1),
   and the 1‑D sequence is obtained by reading a(n,k) along
   antidiagonals (n + k = constant, n increasing from 1). */

/* Required utilities */
#include "../utils/seq_pow.h"

seq_t a(idx_t idx)
{
    /* Find the sum S = n + k of the antidiagonal containing term idx.
       The cumulative number of terms up to sum S is S(S-1)/2. */
    idx_t S = 2;
    for (;;) {
        unsigned __int128 total = (unsigned __int128)(S - 1) * S / 2;
        if ((unsigned __int128)idx <= total)
            break;
        ++S;
    }

    /* Number of terms before this diagonal */
    unsigned __int128 prev_tot = (unsigned __int128)(S - 2) * (S - 1) / 2;
    idx_t prev_terms = (idx_t)prev_tot;

    /* Position within the diagonal (0‑based) */
    idx_t pos = idx - prev_terms - 1; /* pos in [0, S-2] */
    idx_t n = pos + 1;                /* row index */
    idx_t k = S - n;                  /* column index */

    seq_t km1 = (seq_t)(k - 1);

    /* Compute (k-1)^n */
    seq_t pow_term = seq_pow(km1, n);

    /* Add or subtract (k-1) depending on parity of n */
    if (n & 1) /* n odd */
        return pow_term - km1;
    else       /* n even */
        return pow_term + km1;
}
