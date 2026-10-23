/*
 * A110327  Triangle read by rows: T(n,k) = n!*Pell(n-k+1)/k!, where
 *          Pell(n)=A000129(n).
 * https://oeis.org/A110327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) with r = row number, k = column number in the *
 *            triangle read by rows: * T(r,k) = r! * Pell(r-k+1) / k! * where
 *            Pell(0)=0, Pell(1)=1 and Pell(m)=2*Pell(m-1)+Pell(m-2)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   pell_number
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pell_number.h"

/* ------------------------------------------------------------------ */
/* Small helper to compute the Pell number P(m) for 0 ≤ m ≤ 10000.     */

/* ------------------------------------------------------------------ */
/* Return the smallest r such that r*(r+1)/2 > n.  Then row = r-1.     */
/* For the given constraints (n ≤ 10000) a linear scan is fast enough */
static idx_t row_of_n(idx_t n)
{
    idx_t r = 0;
    /* r increases until (r+1)*(r+2)/2 > n */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    return r;   /* this r is equal to row number (0‑based) */
}

/* ------------------------------------------------------------------ */
/* Main function required by the build.                                */
seq_t a(idx_t n)
{
    /* Find row r and column k for index n.                       */
    idx_t r = row_of_n(n);
    idx_t start = r * (r + 1) / 2;  /* first index of this row   */
    idx_t k   = n - start;          /* column index (0‑based)    */

    /* Compute the factorial quotient r! / k! = product(k+1 .. r)   */
    seq_t fact_q = (seq_t)1;
    for (idx_t i = k + 1; i <= r; ++i)
        fact_q *= (seq_t)i;

    /* Pell number with argument (r - k + 1)                       */
    idx_t pell_arg = r - k + 1;
    seq_t pell_val = pell_number(pell_arg);

    /* Final result:  (r! / k!) * Pell(r-k+1)                     */
    return fact_q * pell_val;
}
