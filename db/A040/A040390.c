/*
 * A040390  Continued fraction for sqrt(411).
 * https://oeis.org/A040390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A040390: Continued fraction for sqrt(411)
 *
 * The sequence consists of the integer terms of the simple continued
 * fraction expansion of √411.  The expansion is periodic:
 *
 *   a(0) = ⌊√411⌋ = 20
 *   a(1)...a(L) = the period = (3,1,1,1,19,1,1,1,3,40)
 *
 * Hence for n ≥ 1,
 *
 *   a(n) = period[(n-1) mod L]
 *
 * where L = 10 is the period length.
 *
 * The code below computes the expansion once and then returns the
 * appropriate term for any index n.
 */

#include <stddef.h>   // for size_t

/* Helper: integer square root for 64‑bit values */
static idx_t isqrt_u64(idx_t n) {
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = (low + high) / 2;
        idx_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n)  low = mid + 1;
        else         high = mid - 1;
    }
    return high;
}

/* Main body of the sequence */
seq_t a(idx_t n) {
    /* Periodic part of the continued fraction for √411 */
    static seq_t period[100];
    static size_t period_len = 0;
    static idx_t  a0         = 0;   /* ⌊√411⌋ */

    if (period_len == 0) {          /* init once */

        /* Compute a0 */
        a0 = isqrt_u64(411);        /* a0 = 20 */

        /* Generate the period using the standard continued‑fraction algorithm */
        idx_t m = 0;                /* m0 */
        idx_t d = 1;                /* d0 */
        idx_t a_cur = a0;           /* a0 */

        idx_t count = 0;
        do {
            /* Next terms */
            idx_t m_next = d * a_cur - m;
            idx_t d_next = (411 - m_next * m_next) / d;
            idx_t a_next = (a0 + m_next) / d_next;

            /* Store a_next */
            period[count++] = (seq_t)a_next;

            /* Update */
            m = m_next;
            d = d_next;
            a_cur = a_next;
        } while (a_cur != 2 * a0);   /* period ends when a == 2*a0 */

        period_len = count;           /* should be 10 for √411 */
    }

    if (n == 0) return (seq_t)a0;
    /* Return the corresponding periodic term */
    return period[(size_t)((n - 1) % period_len)];
}
