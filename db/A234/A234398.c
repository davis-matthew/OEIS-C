/*
 * A234398  Irregular table read by rows: T(n, m) = min(4*m+2, n - 2*m^2),
 *          included only when positive; n >= 1, m >= 0.
 * https://oeis.org/A234398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A234398: a(n) = element of the irregular table T(r,m)
 * where T(r,m) = min(4*m+2, r-2*m*m) for r≥1, m≥0, and only positive values
 * are included.  The table is read row‑wise (first all m=0,1,2,… of r=1,
 * then r=2, etc.).  This function returns the n-th term (n≥1). */

#include <stdint.h>

/* integer square‑root floor of a 64‑bit unsigned integer,
 * implemented without using math libraries to keep the code
 * pure C/C++ standard and avoid overflow in intermediate
 * multiplication by using division. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    for (idx_t bit = ((idx_t)1U) << 62; bit > 0; bit >>= 1)
    {
        idx_t candidate = r + bit;
        if (candidate && x / candidate >= candidate)
            r = candidate;
    }
    return r;
}

/* Return the a(n) value. */
seq_t a(idx_t n)
{
    /* n is guaranteed ≥1 (calls outside are undefined). */
    idx_t cumulative = 0;   /* terms processed so far */
    idx_t row = 1;          /* current row index r */

    /* Find the row containing the n‑th term. */
    while (1)
    {
        /* row length: number of m with T(r,m)>0. */
        idx_t half = (row - 1) >> 1;                /* floor((r-1)/2) */
        idx_t m_max = isqrt_u64(half);              /* largest m with 2*m*m<r */
        idx_t len = m_max + 1;                      /* r has m=0..m_max */

        if (cumulative + len >= n)                  /* desired item is here */
            break;

        cumulative += len;                          /* skip whole row */
        ++row;
    }

    /* Position inside the chosen row (0‑based). */
    idx_t pos_in_row = n - cumulative - 1;          /* 0 ≤ pos_in_row ≤ m_max */
    idx_t m = pos_in_row;

    /* Compute T(row,m) = min(4*m+2, row - 2*m*m). */
    seq_t sm = (seq_t)m;
    seq_t t1 = 4 * sm + 2;                           /* 4*m+2   */
    seq_t t2 = (seq_t)row - (seq_t)2 * sm * sm;     /* row-2*m*m */

    return (t1 <= t2) ? t1 : t2;
}
