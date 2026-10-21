/*
 * A229035  Partial sums of A082233 (read sequentially meandering).
 * https://oeis.org/A229035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A229035: partial sums of the meandering read of A082233.
   A082233(m) is the value at the m-th cell of the infinite array
   a_{r,c} = 2r + c - 2, read along anti‑diagonals:  
   - Diagonal d = i + j (starting d = 2).  
   - If d is odd, traverse the diagonal from top to bottom  
     (i = 1 … d‑1, j = d‑i).  
   - If d is even, traverse from bottom to top  
     (i = d‑1 … 1, j = d‑i).  
   a(n) = Σ_{m=1..n} A082233(m).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; return 0 for safety */
    seq_t sum = 0;
    idx_t count = 0;
    for (idx_t d = 2; count < n; ++d) {
        idx_t len = d - 1;          /* number of cells on this diagonal */
        if (d & 1U) {              /* odd diagonal: top to bottom */
            for (idx_t k = 1; k <= len && count < n; ++k) {
                idx_t i = k;
                idx_t j = d - k;
                seq_t val = (seq_t)2 * i + (seq_t)j - 2;
                sum += val;
                ++count;
            }
        } else {                    /* even diagonal: bottom to top */
            for (idx_t k = 1; k <= len && count < n; ++k) {
                idx_t i = d - k;
                idx_t j = k;
                seq_t val = (seq_t)2 * i + (seq_t)j - 2;
                sum += val;
                ++count;
            }
        }
    }
    return sum;
}
