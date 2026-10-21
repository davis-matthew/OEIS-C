/*
 * A059515  Square array T(k,n) by antidiagonals, where T(k,n) is number of
 *          ways of placing n identifiable nonnegative intervals with a total
 *          of exactly k starting and/or finishing points.
 * https://oeis.org/A059515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(k,n) = T(k-2,n-1)*k*(k-1)/2 + T(k-1,n-1)*k*k +
 *            T(k,n-1)*k*(k+1)/2 * with T(0,0)=1 and T(k,n)=0 when k<0 or
 *            k>2n. * a(n) is obtained by reading T(k,n) along antidiagonals:
 *            * for each s=0,1,2,... traverse k from s down to 0 (n=s-k)
 * Range:     u8 n <= 30, u16 n <= 49, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: `seq_t` and `idx_t` are defined by the build. */

#include <stdlib.h>

seq_t a(idx_t N)
{
    /* Find the diagonal s such that (s+1)(s+2)/2 > N. */
    idx_t s = 0;
    while (( (__int128)(s + 1) * (s + 2) ) / 2 <= (__int128)N)
        ++s;

    /* Number of terms before this diagonal. */
    idx_t prev = (s * (s + 1)) / 2;
    idx_t pos   = N - prev;       /* position within the diagonal (0 .. s) */
    idx_t n     = pos;            /* n of the desired cell           */
    idx_t k     = s - pos;        /* k of the desired cell           */

    if (k > 2 * n)
        return (seq_t)0;           /* out of the defined range */

    /* Build rows up to n, keeping only the previous row. */
    seq_t *prev_row = NULL;
    size_t prev_len = 0;          /* length of prev_row (2*(r-1)+1) */

    for (idx_t r = 0; r <= n; ++r)
    {
        size_t len = 2 * r + 1;
        seq_t *curr = (seq_t *)calloc(len, sizeof(seq_t));
        if (!curr) exit(1);       /* allocation failure */

        if (r == 0)
        {
            curr[0] = 1;          /* T(0,0) */
        }
        else
        {
            for (idx_t kk = 0; kk < len; ++kk)
            {
                seq_t val = 0;

                /* term from T(k-2, n-1) */
                if (kk >= 2 && (kk - 2) < prev_len)
                {
                    seq_t coeff = ((seq_t)kk * (seq_t)(kk - 1)) / 2;
                    val += coeff * prev_row[kk - 2];
                }

                /* term from T(k-1, n-1) */
                if (kk >= 1 && (kk - 1) < prev_len)
                {
                    seq_t coeff = (seq_t)kk * (seq_t)kk;
                    val += coeff * prev_row[kk - 1];
                }

                /* term from T(k, n-1) */
                if (kk < prev_len)
                {
                    seq_t coeff = ((seq_t)kk * (seq_t)(kk + 1)) / 2;
                    val += coeff * prev_row[kk];
                }

                curr[kk] = val;
            }
        }

        /* Free previous row and move current to prev_row. */
        if (prev_row) free(prev_row);
        prev_row = curr;
        prev_len = len;
    }

    /* The desired value is at index k of the last row (n). */
    seq_t result = prev_row[k];
    free(prev_row);
    return result;
}
