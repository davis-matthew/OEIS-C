/*
 * A051497  (Terms in A014476)/2.
 * https://oeis.org/A051497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 45, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A051497: Half of the even binomial coefficients C(m,k), 
   ordered by increasing m and decreasing k, where 0 < k < m and
   k ≤ floor((m−1)/2).  The n‑th term a(n) is the n‑th value in this
   sequence.  Each term is a(n) = C(m,k) / 2 for the corresponding (m,k).  */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of terms produced so far   */
    idx_t m = 4;              /* starting row in Pascal's triangle */

    /* Generate terms on the fly until the n‑th term is found. */
    while (1)
    {
        /* compute maximum k in this row (we only need k ≤ floor((m−1)/2)) */
        idx_t k_max = (m - 1) / 2;
        if (k_max > 0)
        {
            /* compute C(m, k_max) multiplicatively */
            seq_t cur = 1;
            for (idx_t i = 1; i <= k_max; ++i)
            {
                cur = (cur * (seq_t)(m - k_max + i)) / (seq_t)i;
            }

            /* iterate k from k_max down to 1, generating terms */
            for (idx_t k = k_max; k >= 1; --k)
            {
                /* C(m,k) is odd iff all bits of k are subsets of those of m
                   (Lucas theorem for mod 2).  We keep the element only when
                   it is even. */
                if ((k & ((idx_t)~m)) != 0)   /* even binomial coefficient */
                {
                    ++count;
                    if (count == n)
                        return cur / 2;      /* a(n) is half of the binomial */
                }

                /* move to the next smaller k; skip update when k == 1
                   (loop will terminate soon). */
                if (k > 1)
                {
                    cur = (cur * (seq_t)k) / (seq_t)(m - k + 1);
                }
            }
        }
        ++m;                       /* proceed to the next row */
    }
}
