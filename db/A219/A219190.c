/*
 * A219190  Numbers of the form k*(5*k+1), where k = 0,-1,1,-2,2,-3,3,...
 * https://oeis.org/A219190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n*(n-1) + 3*(-1)^n*(2*n - 1) + 3)/8
 * Range:     u8 n <= 15, u16 n <= 229, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use signed 128‑bit arithmetic to avoid overflow and treat
       negative intermediate values correctly.  The final value is
       always non‑negative, so a cast to seq_t yields the correct result.  */
    __int128 nn = (__int128)n;
    __int128 term = 10 * nn * (nn - 1);

    if (n & 1)          /* n is odd  =>  subtract 3*(2*n-1)  */
        term -= 3 * (2 * nn - 1);
    else                /* n is even =>  add    3*(2*n-1)  */
        term += 3 * (2 * nn - 1);

    term += 3;          /* final +3 from the formula */

    return (seq_t)(term / 8);   /* integer division is exact */
}
