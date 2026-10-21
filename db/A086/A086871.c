/*
 * A086871  Row sums of A059450.
 * https://oeis.org/A086871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A086871 – row sums of A059450
 *
 *  Recurrence:
 *      a(0) = 1
 *      a(1) = 2
 *      for n ≥ 2
 *          (n+1)*a(n) = 5*(2*n-1)*a(n-1) – 9*(n-2)*a(n-2)
 *
 *  The implementation uses a 128‑bit integer for the intermediate
 *  arithmetic when seq_t is 64‑bit (or less).  For larger seq_t
 *  types the operations are performed directly on seq_t.
 *
 *  No caching is used – each call recomputes the values up to n.
 *  This is O(n) per call and well under the 2 s limit for n≈10 000.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle the two base cases */
    if (n == (idx_t)0)
        return (seq_t)1;
    if (n == (idx_t)1)
        return (seq_t)2;

    /* a(n-2) and a(n-1) */
    seq_t a_prev2 = (seq_t)1;  /* a(0) */
    seq_t a_prev1 = (seq_t)2;  /* a(1) */
    seq_t a_curr  = 0;

    /* use 128‑bit intermediate only when seq_t is 64‑bit (or less) */
    if (sizeof(seq_t) <= 8) {
        for (idx_t i = (idx_t)2; i <= n; ++i) {
            /* cast operands to 128‑bit to avoid overflow */
            unsigned __int128 term1 = (unsigned __int128)5 *
                                      (unsigned __int128)((unsigned __int128)2*i - 1) *
                                      (unsigned __int128)a_prev1;
            unsigned __int128 term2 = (unsigned __int128)9 *
                                      (unsigned __int128)(i - 2) *
                                      (unsigned __int128)a_prev2;
            unsigned __int128 num   = term1 - term2;
            a_curr = (seq_t)(num / ((unsigned __int128)i + 1));

            a_prev2 = a_prev1;
            a_prev1 = a_curr;
        }
    }
    else {   /* seq_t is larger (arbitrary‑precision) */
        for (idx_t i = (idx_t)2; i <= n; ++i) {
            seq_t term1 = (seq_t)5 *
                          ((seq_t)(2*i - 1) * a_prev1);
            seq_t term2 = (seq_t)9 *
                          ((seq_t)(i - 2) * a_prev2);
            seq_t num   = term1 - term2;
            a_curr = num / ((seq_t)i + 1);

            a_prev2 = a_prev1;
            a_prev1 = a_curr;
        }
    }

    return a_curr;
}
