/*
 * A191566  a(n) = 7*a(n-1) + (-1)^n*6*2^(n-1).
 * https://oeis.org/A191566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(n)=7*a(n-1)+(-1)^n*6*2^(n-1)  (n≥1) */

seq_t a(idx_t n)
{
    seq_t a_prev = (seq_t)1;   /* a(0) */
    seq_t pow2   = (seq_t)1;   /* 2^(i-1), starts with i=1 */

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        seq_t term   = (seq_t)6 * pow2;   /* 6*2^(i-1) */
        seq_t newval = (seq_t)7 * a_prev; /* 7*a(n-1) */

        if (i & (idx_t)1)   /* odd i: subtract term */
            newval -= term;
        else                 /* even i: add term   */
            newval += term;

        a_prev = newval;
        pow2 <<= 1;        /* prepare 2^i for next iteration */
    }

    return a_prev;        /* a(n) */
}
