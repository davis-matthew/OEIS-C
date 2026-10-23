/*
 * A117742  Triangle read by rows: coefficient of x^n in the Taylor expansion
 *          of x/(1 - m*x - x^4) in row n, column m=1..n+2.
 * https://oeis.org/A117742
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of x^n in x/(1 - m*x - x^4) read by rows (row
 *            n, columns m=1..n+2), then flattened. The sequence starts at
 *            a(-1)=0
 * Range:     u8 n <= 22, u16 n <= 40, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* handle indices below the offset */
    if (n < -1) return (seq_t)0;
    if (n == -1) return (seq_t)0;

    int64_t n64 = n;           /* work in native integers for indices */

    /* determine the row r (exponent) from the linear index n */
    int64_t r;
    double d = sqrt((double)(9 + 8 * n64));
    r = (int64_t)((-3 + d) / 2);

    /* adjust in case of off‑by‑one due to rounding */
    while ((r + 1) * (r + 4) / 2 <= n64) ++r;
    while (r * (r + 3) / 2 > n64) --r;

    /* start index of this row in the flattened sequence */
    int64_t start = r * (r + 3) / 2;
    int64_t offset = n64 - start;   /* 0‑based offset within the row */
    int64_t m = offset + 1;         /* column (1‑based) */

    /* compute a_m(r) by the recurrence:
       a(0)=0, a(1)=1, a(2)=m, a(3)=m^2,
       a(k)=m*a(k-1)+a(k-4) for k>=4 */
    if (r == 0) return (seq_t)0;
    if (r == 1) return (seq_t)1;

    seq_t m_seq = (seq_t)m;
    seq_t a0 = 0;                   /* a(k-4) */
    seq_t a1 = 1;                   /* a(k-3) */
    seq_t a2 = m_seq;               /* a(k-2) */
    seq_t a3 = m_seq * m_seq;       /* a(k-1) */

    if (r == 2) return a2;
    if (r == 3) return a3;

    for (int64_t k = 4; k <= r; ++k) {
        seq_t a_next = m_seq * a3 + a0;   /* a(k) = m*a(k-1)+a(k-4) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a_next;
    }
    return a3;
}
