/*
 * A047971  Triangle of coefficients of Gaussian polynomials [ n+3,3 ].
 * https://oeis.org/A047971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, tabf
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A047971: flattened triangle of coefficients of the Gaussian
    polynomial [n+3,3]_q.  The term a(n) is the (n+1)-st entry when the
    triangle T(p,w) (p≥0, w=0…3p) is read row by row.
    The value T(p,w) equals the number of solutions of
        m0+m1+m2+m3 = p ,   1·m1 + 2·m2 + 3·m3 = w ,
    i.e. the number of partitions of w into at most p parts each ≤3. */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */

/*--- helper --------------------------------------------------------------*/
/*  Count solutions (m1,m2,m3) with the given constraints.  */
static seq_t count_T(idx_t p, idx_t w)
{
    /* Count of solutions of:
            m0 + m1 + m2 + m3 = p
            m1 + 2 m2 + 3 m3 = w
       with m0..m3 ≥ 0.
    */
    idx_t count = 0;
    for (idx_t m3 = 0;
         m3 <= p && m3 * 3 <= w;
         ++m3)
    {
        idx_t wprime = w - 3 * m3;              /* remaining weight for m1,m2 */
        idx_t max_m2 = wprime / 2;              /* 2*m2 ≤ wprime            */
        if (max_m2 > (p - m3))
            max_m2 = p - m3;                    /* m2 + m3 ≤ p                */
        /* m2 must satisfy   wprime - 2*m2 ≥ 0   and
           wprime - 2*m2 ≤ p - m3 - m2   ->
           m2 ≥ wprime - (p - m3) */
        idx_t min_m2 = wprime > (p - m3) ? wprime - (p - m3) : 0;
        if (min_m2 <= max_m2)
            count += (max_m2 - min_m2 + 1);
    }
    return (seq_t)count;
}

/*--- main sequence function ------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Find the row p such that the cumulative number of terms up to
       row p-1 is ≤ n < cumulative up to row p.   */
    idx_t p = 0;          /* current row */
    size_t rowsz = 1;     /* length of current row = 3p+1 */
    size_t upto = 0;      /* cumulative count of terms up to previous row */

    while (upto + rowsz <= n)
    {
        ++p;
        rowsz = 3 * p + 1;
        upto += rowsz - 3;   /* rowsz of previous row = 3(p-1)+1; but we already added 3? */
        /* The above update keeps upto as the sum of lengths of rows < p. */
    }

    /* At this point:
       - p is the desired row (0‑based)
       - upto is the cumulative number of terms before row p
    */
    size_t w = (size_t)(n - upto);   /* column within the row p */

    return count_T(p, w);
}
