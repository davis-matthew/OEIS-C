/*
 * A036838  Triangle read by rows: T(n,k) = value of Schoenheim bound
 *          L_1(n+2,k+2,k+1) on covering numbers (0 <= k <= n).
 * https://oeis.org/A036838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Triangle read by rows: T(n,k) = value of Schoenheim bound L_1(n+2,k+2,k+1)  
   on covering numbers (0 ≤ k ≤ n).  
   a(n) is the flattened triangle by reading rows from left to right. 
   Implementation uses the recursive definition  
      L_1(v,k,1) = ceil(v/k)  
      L_1(v,k,t) = ceil(v/k * L_1(v-1,k-1,t-1))          (t>1)  
   The routine loops over the factors in the correct order (from the
   innermost recursion outward) and applies the ceil after each step.
*/
seq_t a(idx_t n)
{
    /* find row r and column c so that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    idx_t c = n - r * (r + 1) / 2;

    /* compute L_1(r+2, c+2, c+1) */
    seq_t val = 1;
    for (idx_t j = 0; j <= c; ++j) {
        idx_t i = c - j;                 /* i decreases from c to 0 */
        seq_t num = (seq_t)(r + 2 - i);   /* numerator (v-i) */
        seq_t den = (seq_t)(c + 2 - i);   /* denominator (k-i) */
        val = (num * val + den - 1) / den;/* ceil division */
    }
    return val;
}
