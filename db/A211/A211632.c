/*
 * A211632  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...-1,1,...,n} and 2w^2>x^2+y^2.
 * https://oeis.org/A211632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*∑_{w=1}^{n}∑_{a=1}^{n} min(n,⌊√(2w²-1-a²)⌋) The formula
 *            counts ordered triples (w,x,y) with w,x,y∈{±1,…,±n}, satisfying
 *            2w² > x² + y²
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t w = 1; w <= n; ++w) {
        idx_t limit = 2 * w * w - 1;        /* 2w² - 1  (strict inequality) */
        idx_t b = n;                        /* maximal possible |y| */
        idx_t sumPairs = 0;
        for (idx_t a = 1; a <= n; ++a) {
            /* Decrease b until a² + b² ≤ limit. */
            while (b > 0 && a * a + b * b > limit) {
                --b;
            }
            if (b == 0) break;             /* further a will only increase a² */
            sumPairs += b;                  /* all (a,b) with this a count */
        }
        total += (seq_t)sumPairs;
    }
    total *= 8;                              /* account for signs of w,x,y */
    return total;
}
