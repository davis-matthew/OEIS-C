/*
 * A211544  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          2w=3x-5y.
 * https://oeis.org/A211544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of ordered triples (w,x,y) with 1≤w,x,y≤n such
 *            that 2w = 3x – 5y
 * Range:     u8 n <= 44, u16 n <= 701, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t w = 1; w <= n; ++w)
    {
        /* y must satisfy 3x = 2w + 5y, 1 ≤ x ≤ n.
           Hence y ≤ (3n – 2w)/5. */
        idx_t y_max = (3 * n - 2 * w) / 5;
        if (y_max < 1)                      /* no possible y for this w */
            continue;

        /* Congruence condition: 2w + 5y ≡ 0 (mod 3)
           => y ≡ –w (mod 3). */
        idx_t r = (3 - (w % 3)) % 3;         /* required residue (0,1,2) */

        idx_t first_y = r;                  /* smallest positive y with this residue */
        if (first_y == 0)
            first_y = 3;                    /* 0 is not in the allowed range [1,n] */

        if (first_y > y_max)
            continue;                       /* no y in the range */

        idx_t cnt = (y_max - first_y) / 3 + 1;   /* number of terms of the AP */
        total += (seq_t)cnt;
    }
    return total;
}
