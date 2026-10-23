/*
 * A156173  A q-factorial type triangle sequence: t(n,m)=Product[Sum[(m +
 *          1)^i, {i, 0, k - 1}], {k, 1, n}].
 * https://oeis.org/A156173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 
 * Formula: a(n) = t(r,c) where r and c are the row and column indices of the 
 * triangle entry with serial number n.  The triangle is defined by    
 * t(n,m) = ∏_{k=1}^{n} Σ_{i=0}^{k-1} (m+1)^i
 *                                 = ∏_{k=1}^{n} ((m+1)^k - 1) / m .
 * The function converts the 1‑based index n into (r,c) and evaluates t(r,c).
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n<1, return 0 */

    /* Find the row r such that r*(r+1)/2 ≥ n */
    idx_t r = 1;
    idx_t cum = 1;             /* cumulative count of entries up to row r */
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* Column c within the r‑th row (1‑based) */
    idx_t prev_cumulative = cum - r;   /* total entries before row r */
    idx_t c = n - prev_cumulative;     /* column in the current row */

    seq_t m   = (seq_t)c;             /* use as the column index (m) */
    seq_t prod = 1;                   /* running product for t(r,c) */
    seq_t base = (seq_t)(c + 1);       /* (m+1) */
    seq_t powk = 1;                   /* (m+1)^k, will be updated each loop */

    for (idx_t k = 1; k <= r; ++k) {
        powk *= base;                 /* (m+1)^k */
        seq_t pk = (powk - 1) / m;    /* ((m+1)^k - 1)/m */
        prod *= pk;
    }
    return prod;
}
