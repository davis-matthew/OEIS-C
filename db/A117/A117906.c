/*
 * A117906  Inverse of number triangle A117904.
 * https://oeis.org/A117906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 1325, i16 n <= 1325, i32 n <= 1325, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A117906: Inverse of number triangle A117904.  
   The triangle entries T(r,c) are defined by the rational generating
   function  

        (1 -x*(1-y) +x^2*y^2 -x^3*y -x^5*y^2)/(1-x^3*y^3).

   From this one obtains

        T(r,c) =
           1  if (r,c) = (3k,3k), (1,1)+3k, (2,2)+3k
          -1  if (r,c) = (3k+1,3k), (3k+3,3k+1), (3k+5,3k+2)
           0  otherwise  (k ≥ 0 integer).

   The function a(n) returns the row‑major flattened value of this
   triangle:
        a(0)=1, a(1)=-1, a(2)=1, a(3)=0, a(4)=0, a(5)=1, …          */

seq_t a(idx_t n)
{
    /* Determine the row r so that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.  */
    idx_t r = 0;
    idx_t tri = 0;                 /* triangle number r*(r+1)/2 */
    while (tri + (r + 1) <= n) {
        tri += r + 1;
        r++;
    }

    /* Column within that row.  */
    idx_t c = n - tri;

    /* Remove full blocks of size 3 in both coordinates.  */
    idx_t k = (r < c ? r : c) / 3;
    idx_t r2 = r - 3 * k;
    idx_t c2 = c - 3 * k;

    /* Match the few non‑zero patterns.  */
    if (r2 == 0 && c2 == 0) return (seq_t)1;
    if (r2 == 1 && c2 == 1) return (seq_t)1;
    if (r2 == 2 && c2 == 2) return (seq_t)1;
    if (r2 == 1 && c2 == 0) return (seq_t)(-1);
    if (r2 == 3 && c2 == 1) return (seq_t)(-1);
    if (r2 == 5 && c2 == 2) return (seq_t)(-1);

    return (seq_t)0;
}
