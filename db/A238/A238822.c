/*
 * A238822  Number of equilateral triangles bounded by the sides and diagonals
 *          of a regular 3n-gon.
 * https://oeis.org/A238822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   For odd n: a(n) = n·(n+1)^3 / 2 – 3·n² For even n: a(n) =
 *            n·(n+1)^3 / 2 – 3·n² – [ 6·(n/2)² – 5·(n/2) + 12·(n/2)·[ (n/2) %
 *            5 == 0 ] ] where [condition] is 1 if the condition is true,
 *            otherwise 0
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* OEIS offset is 1; no specification for n==0 – return 0. */
    if (n == 0)
        return 0;

    /* Common part:
       f = n·(n+1)^3 / 2 – 3·n²                         */
    seq_t n_t  = (seq_t)n;
    seq_t n1   = (seq_t)(n + 1);
    seq_t n1_sq = n1 * n1;                       /* (n+1)^2   */
    seq_t n1_cu = n1_sq * n1;                    /* (n+1)^3   */
    seq_t f  = n_t * n1_cu / 2;                  /* division by 2 */
    f -= 3 * n_t * n_t;                          /* subtract 3*n² */

    /* Odd n: return f.                                         */
    if (n & 1)
        return f;

    /* Even n: subtract the correction term.                     */
    idx_t m = n / 2;                            /* m = n/2       */
    seq_t m_t = (seq_t)m;

    /* diff = 6*m^2 – 5*m + 12*m*χ_{5|m}                        */
    seq_t diff = (seq_t)6 * m_t * m_t;           /* 6·m²          */
    diff -= (seq_t)5 * m_t;                      /* –5·m          */
    if ((m % 5) == 0)                           /* add 12·m if m≡0 (mod 5) */
        diff += (seq_t)12 * m_t;

    return f - diff;
}
