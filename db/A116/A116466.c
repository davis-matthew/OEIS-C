/*
 * A116466  Unsigned row sums of triangle A114700.
 * https://oeis.org/A116466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ( (H_n + 2*H_{n-1}) - a(n-2) ) / 2
 * where H_n = 2            (n = 0)
 *          4 + binom(2n-2, n-1)  (n even >= 2)
 *          0                     (n odd)
 *
 * The recurrence follows from the generating function
 * A(x) = (1+2x)*( 2*(1+x^2)/(1-x^2) + x^2/(1-4x^2)^(1/2) )/(2+x^2).
 * It requires only integer arithmetic and runs in O(n) time with
 * O(n) memory.
 */

#include <stdlib.h>
#include <stddef.h>

/* The function header as required by the build system */
seq_t a(idx_t n)
{
    /* Allocate an array to store the sequence values up to n. */
    seq_t *seq = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!seq) /* Allocation failure: return 0 as a safe fallback. */
        return 0;

    seq_t h_prev = 0;        /* H_{i-1} */
    seq_t c = 0;             /* binom(2m-2, m-1) for current even i (m>0) */
    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t h_curr;        /* H_i */

        /* Compute H_i depending on the parity of i. */
        if ((i & 1u) == 0)            /* i even */
        {
            idx_t m = i >> 1;         /* m = i/2 */

            if (m == 0)               /* i == 0 */
                h_curr = 2;
            else
            {
                if (m == 1)            /* m == 1 -> binom(0,0) = 1 */
                    c = 1;
                else
                {
                    /* recurrence:
                     * binom(2m-2, m-1) = binom(2(m-1)-2, (m-1)-1) *
                     *                     (4*(m-1)-2) / (m-1)
                     */
                    seq_t numerator = c * (4 * (m - 1) - 2);
                    c = numerator / (m - 1);
                }
                h_curr = 4 + c;
            }
        }
        else                            /* i odd */
        {
            h_curr = 0;
        }

        /* Right-hand side of the recurrence: r_i = H_i + 2*H_{i-1}. */
        seq_t r = h_curr + (h_prev << 1);   /* 2*H_{i-1} */

        /* Previous value a_{i-2} (0 if i<2). */
        seq_t prev2 = (i >= 2) ? seq[i - 2] : 0;

        /* Compute a_i = (r - a_{i-2}) / 2.  The result is always an integer. */
        seq_t numerator = r - prev2;
        seq[i] = numerator >> 1;           /* divide by 2 */

        /* Prepare for next iteration. */
        h_prev = h_curr;
    }

    /* Result at index n.  Free the temporary array. */
    seq_t result = seq[n];
    free(seq);
    return result;
}
