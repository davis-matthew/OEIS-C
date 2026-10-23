/*
 * A131621  Sum of all n-digit Pell numbers.
 * https://oeis.org/A131621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of all n‑digit Pell numbers.  
   Pell numbers: P(0)=0, P(1)=1, P(k)=2*P(k‑1)+P(k‑2).  
   a(n) = Σ P(k) over all k such that digit_len(P(k))==n. */
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for the offset 1 */

    seq_t p_prev = 0;   /* P(0) */
    seq_t p_curr = 1;   /* P(1) */
    seq_t sum    = 0;
    int d;           /* decimal digit length of a Pell number */

    /* Check the first two Pell numbers. */
    d = digit_len(p_prev);
    if (d == n) sum += p_prev;
    d = digit_len(p_curr);
    if (d == n) sum += p_curr;

    /* Generate remaining Pell numbers until the digit length
       exceeds the requested n. */
    while (1)
    {
        seq_t p_next = (p_curr << 1) + p_prev;  /* 2*P(k-1)+P(k-2) */

        d = digit_len(p_next);
        if (d > n) break;                      /* no further numbers with n digits */

        if (d == n) sum += p_next;

        p_prev = p_curr;
        p_curr = p_next;
    }

    return sum;
}
