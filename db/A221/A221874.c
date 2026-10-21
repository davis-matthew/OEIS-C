/*
 * A221874  Numbers m such that 10*m^2 + 6 is a square.
 * https://oeis.org/A221874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A221874: Numbers m such that 10*m^2 + 6 is a square.
 *
 * The solutions of 10*m^2 + 6 = y^2 are obtained from the Pell equation
 * y^2 - 10*m^2 = 6.  Let u = 19 + 6√10 (the fundamental unit of
 * x^2 - 10y^2 = 1).  For k ≥ 0 let
 *
 *   U_k = u^k = a_k + b_k √10      (a_k, b_k ∈ ℕ)
 *
 * Then:
 *   - for odd n = 2k+1 :   m = a_k + 4·b_k   (multiply (4+√10) by U_k)
 *   - for even n = 2k+2 :  m = 5·a_k + 16·b_k (multiply (16+5√10) by U_k)
 *
 * The pair (a_k,b_k) satisfies the linear recurrences
 *
 *   a_{k+1} = 19·a_k + 60·b_k
 *   b_{k+1} =  6·a_k + 19·b_k
 *
 * which are implemented below.  This is an O(k) algorithm with k≈n/2,
 * far below the 2‑second requirement for n up to 10 000.
 *
 * Parameters:
 *   n – index (1‑based, n ≥ 1)
 * Returns:
 *   a(n) – the n‑th term of the sequence, as an object of type seq_t.
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety, not used for valid n */

    idx_t k;
    if (n & 1)                     /* odd n = 2k+1 */
        k = (n - 1) >> 1;          /* k = (n-1)/2 */
    else                           /* even n = 2k+2 */
        k = (n - 2) >> 1;          /* k = (n-2)/2 */

    /* start with U_0 = 1 + 0*√10 */
    seq_t a_val = 1, b_val = 0;

    /* iterate k times: (a,b) = (19a + 60b , 6a + 19b) */
    for (idx_t i = 0; i < k; ++i) {
        seq_t new_a = a_val * (seq_t)19 + b_val * (seq_t)60;   /* 10*6 = 60 */
        seq_t new_b = a_val * (seq_t)6  + b_val * (seq_t)19;
        a_val = new_a;
        b_val = new_b;
    }

    /* compute m from the appropriate formula */
    if (n & 1)   /* odd */
        return a_val + b_val * (seq_t)4;
    else         /* even */
        return a_val * (seq_t)5 + b_val * (seq_t)16;
}
