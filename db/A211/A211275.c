/*
 * A211275  Number of integer pairs (x,y) such that 0 < x <= y <= n and x*y <=
 *          floor(n/2).
 * https://oeis.org/A211275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes OEIS A211275: 
 * a(n) = #{ (x,y) : 0 < x <= y <= n,  x*y <= floor(n/2) }
 *
 * Algorithm: For each x=1..n, the largest y satisfying x*y <= m=floor(n/2) is
 *             y_max = min(n, floor(m/x)).  
 *             If y_max >= x, the number of admissible y for this x is
 *             y_max - x + 1.  
 * The sum of these counts over all x gives a(n).
 */

seq_t a(idx_t n)
{
    /* handle offset: a(1) is the first term; for n == 0 return 0 (unused) */
    if (n == 0)
        return 0;

    seq_t    count = 0;
    idx_t    m     = n / 2;   /* floor(n/2) fits in idx_t */

    for (idx_t x = 1; x <= n; ++x) {
        idx_t y_max = m / x;           /* floor(m/x) */
        if (y_max > n)
            y_max = n;                 /* enforce y <= n */
        if (y_max >= x) {
            count += (seq_t)(y_max - x + 1);
        }
    }

    return count;
}
