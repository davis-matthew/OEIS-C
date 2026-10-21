/*
 * A092879  Triangle of coefficients of the product of two consecutive
 *          Fibonacci polynomials.
 * https://oeis.org/A092879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A092879 – Triangle of coefficients of the product of two consecutive Fibonacci polynomials
 *
 *  The triangle T(n,k) satisfies
 *      T(0,0) = 1
 *      T(n,k) = 0  for k<0 or k>n
 *      T(n,k) = T(n-1,k) + T(n-1,k-1) + T(n-2,k-1) + T(n-2,k-2) - T(n-3,k-3)   (n≥3)
 *
 *  The sequence a(n) is the row‑major flattening of T:
 *      a(n) = T(r,k)   where r is the unique row with r(r+1)/2 ≤ n < (r+1)(r+2)/2
 *      k = n - r(r+1)/2
 *
 *  The implementation below builds the triangle lazily in a static 2‑D array.
 *  It is static‑size but large enough to generate all values needed for
 *  n up to at least 10000 (which requires rows < 150).  All operations use
 *  the integer type seq_t; the recurrence never produces negative
 *  intermediate values, so unsigned arithmetic is safe.
 */

#include <stdint.h>          /* for NULL, if needed */

#define MAX_ROWS 200          /* sufficient for n ≤ 10000 (row ≤ 141) */

/* static storage for the triangle */
static seq_t tri[MAX_ROWS][MAX_ROWS];
/* number of rows that have been computed so far (each row *fully* available) */
static idx_t computed_rows = 0;

/* ensure that rows 0…r are computed */
static void ensure_rows(idx_t r)
{
    if (r >= MAX_ROWS)
        return;   /* safeguard – input n should not be this large */

    while (computed_rows <= r)
    {
        idx_t i = computed_rows;   /* row index to compute */

        if (i == 0)
        {
            tri[0][0] = 1;
        }
        else if (i == 1)
        {
            tri[1][0] = 1;
            tri[1][1] = 1;
        }
        else if (i == 2)
        {
            tri[2][0] = 1;
            tri[2][1] = 3;
            tri[2][2] = 2;
        }
        else   /* i ≥ 3 */
        {
            for (idx_t k = 0; k <= i; ++k)
            {
                seq_t val = 0;

                /* T(i-1,k) */
                if (k <= i-1)
                    val += tri[i-1][k];

                /* T(i-1,k-1) */
                if (k > 0)          /* k-1 ≥ 0 */
                    val += tri[i-1][k-1];

                /* T(i-2,k-1) – only if k-1 ≤ i-2 */
                if (i >= 2 && k > 0 && (k-1) <= (i-2))
                    val += tri[i-2][k-1];

                /* T(i-2,k-2) – only if k-2 ≤ i-2 */
                if (i >= 2 && k > 1)   /* k-2 ≥ 0 automatically when k>1 */
                    val += tri[i-2][k-2];

                /* subtract T(i-3,k-3) – only if k ≥ 3 */
                if (i >= 3 && k >= 3)
                    val -= tri[i-3][k-3];

                tri[i][k] = val;
            }
        }

        ++computed_rows;
    }
}

/* locate the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
static idx_t row_of(idx_t n)
{
    idx_t r = 0;
    while ( (r + 1) * (r + 2) / 2 <= n )
        ++r;
    return r;
}

/* public interface required by the build */
seq_t a(idx_t n)
{
    idx_t r = row_of(n);          /* row containing the requested element */
    idx_t k = n - r * (r + 1) / 2;/* column within that row */

    ensure_rows(r);               /* generate rows up to r if necessary */

    return tri[r][k];
}
