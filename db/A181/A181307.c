/*
 * A181307  Triangle read by rows: T(n,k) is the number of 2-compositions of n
 *          having k columns with only nonzero entries (0<=k<=floor(n/2)).
 * https://oeis.org/A181307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   binom, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A181307: Triangle of 2‑compositions of n by number of columns with both entries nonzero.  
   The function returns the nth entry of the triangle read by rows.  
   For each row n the number of columns is floor(n/2)+1; the entries in the row are T(n,0), T(n,1), …  
   Formulas:  
     T(n,0) = 2*3^(n-1)           (n ≥ 1), T(0,0)=1  
     T(n,1) = (n-1)*3^(n-2)        (n ≥ 2)  
     For k ≥ 2:  
         T(n,k) = Σ_{a=0}^{n-2k}  binom(k+a, k) * 3^a * binom(k-2 + (n-2k-a), k-2)   (n ≥ 2k)  */

#include <stdbool.h>
#include "../utils/pow3.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r and column k corresponding to the linear index n. */
    idx_t cur = n;
    idx_t r   = 0;
    while (true) {
        idx_t len = (r >> 1) + 1;   /* floor(r/2) + 1 */
        if (cur < len) break;
        cur -= len;
        r++;
    }
    idx_t k = cur;   /* column index within the row */

    /* Compute T(r,k). */
    if (k == 0) {
        if (r == 0) return (seq_t)1;
        return (seq_t)2 * pow3(r - 1);
    }
    if (k == 1) {
        return (seq_t)(r - 1) * pow3(r - 2);
    }

    /* k >= 2 */
    idx_t s = r - 2 * k;   /* n - 2k, guaranteed >= 0 */
    seq_t total = 0;

    for (idx_t a = 0; a <= s; ++a) {
        seq_t term = binom(k + a, k) *
                     pow3(a) *
                     binom(k - 2 + (s - a), k - 2);
        total += term;
    }
    return total;
}
