/*
 * A165516  Perfect squares (A000290) that can be expressed as the sum of
 *          three consecutive triangular numbers (A000217).
 * https://oeis.org/A165516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes OEIS A165516 – perfect squares that are sums of three consecutive triangular numbers.
   Sequence derived from the Pell‑type equation 2X²−3Y²=5 with X=2s, Y=2k+3.  
   Two interleaved subsequences are obtained by multiplying the two fundamental solutions
   (U,Y)=(8,3) and (U,Y)=(32,13) by the fundamental unit ε=5+2√6.
   For odd n = 2t−1 use (8,3) multiplied t−1 times; for even n = 2t use (32,13) multiplied t−1 times.
   Result a(n)= (U/4)².
   The algorithm is O(n) and works for any integer type seq_t (arbitrary precision or unsigned integer). 
*/

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined, safety guard */
    if (n == 1)
        return (seq_t)4;          /* base case from U=8 */

    /* Fundamental solutions of U² − 6Y² = 10:
       (U0,Y0) = (8,3)   → odd indices
       (U0,Y0) = (32,13) → even indices
       Fundamental unit ε = 5 + 2√6
    */
    if (n % 2 == 0) {            /* even n = 2t  */
        idx_t t = n / 2;         /* t ≥ 1 */
        seq_t U = 32, Y = 13;    /* U0, Y0 for even case */
        for (idx_t i = 1; i < t; ++i) {
            seq_t U_new = 5 * U + 12 * Y;   /* U' = 5U + 12Y  */
            seq_t Y_new = 5 * Y + 2 * U;    /* Y' = 5Y + 2U   */
            U = U_new;
            Y = Y_new;
        }
        seq_t q = U / 4;          /* U divisible by 4 */
        return q * q;             /* a = (U/4)² */
    } else {                     /* odd n = 2t−1  */
        idx_t t = (n + 1) / 2;    /* t ≥ 1 */
        seq_t U = 8, Y = 3;       /* U0, Y0 for odd case */
        for (idx_t i = 1; i < t; ++i) {
            seq_t U_new = 5 * U + 12 * Y;   /* U' = 5U + 12Y  */
            seq_t Y_new = 5 * Y + 2 * U;    /* Y' = 5Y + 2U   */
            U = U_new;
            Y = Y_new;
        }
        seq_t q = U / 4;          /* U divisible by 4 */
        return q * q;             /* a = (U/4)² */
    }
}
