/*
 * A239610  Sin(arcsin(n) - 4 arccos(n))^2.
 * https://oeis.org/A239610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)^2 * (16*(n-1)^4 - 20*(n-1)^2 + 5)^2
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 6, u64 n <= 49, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (This is the OEIS definition adjusted for offset 1) */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle indices below the offset – return 0 for safety */
    if (n < 1)
        return 0;

    /* Use (n-1) because the OEIS formula is 0‑based but the offset is 1 */
    seq_t m   = (seq_t)(n - 1);      /* m = n-1 */
    seq_t m_sq = m * m;              /* m^2   */
    seq_t m_qu = m_sq * m_sq;        /* m^4   */

    seq_t t = (seq_t)16 * m_qu       /* 16*m^4 */
            - (seq_t)20 * m_sq        /* -20*m^2 */
            + (seq_t)5;               /* +5      */

    seq_t t_sq = t * t;              /* (16*m^4 - 20*m^2 + 5)^2 */
    return m_sq * t_sq;             /* m^2 * t_sq */
}
