/*
 * A112668  Triangle read by rows: row n gives an n-term geometric progression
 *          with first term 1 such that the sum of the n terms is a multiple
 *          of n.
 * https://oeis.org/A112668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the term at position n when a triangle is read row‑wise.
 *            Row r (r ≥ 1) has r terms forming a geometric progression 1, b,
 *            b², …, b^{r‑1} where b is the smallest integer ≥ 2 such that
 *            (b^{r} – 1)/(b – 1) is divisible by r (for r = 1 any b works).
 *            Let prev = r(r – 1)/2 (number of terms in al...
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 64, u64 n <= 152,
 *            u128 n <= 433, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety; offset starts at 1 */

    /* ----- find row r such that r(r+1)/2 ≥ n > (r-1)r/2 ----- */
    idx_t r = 1;
    idx_t cum = r * (r + 1) / 2;
    while (cum < n) {
        ++r;
        cum = r * (r + 1) / 2;
    }

    idx_t prev = (r - 1) * r / 2;
    idx_t pos  = n - prev - 1;           /* zero‑based index within the row */

    /* ----- cache of base values for each row ----- */
    static idx_t cache_cap = 0;          /* largest r cached so far   */
    static seq_t *bases    = NULL;       /* array bases[1 .. cache_cap] */

    if (r > cache_cap) {
        /* allocate larger array */
        idx_t new_cap   = r + 1;                 /* index from 0..r */
        seq_t *new_bases = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
        if (!new_bases) return 0;               /* out of memory: fallback */

        /* copy old cached bases if any */
        if (bases) {
            for (idx_t i = 0; i <= cache_cap; ++i)
                new_bases[i] = bases[i];
            free(bases);
        }

        /* compute base for new rows m = cache_cap+1 .. r */
        for (idx_t m = cache_cap + 1; m <= r; ++m) {
            seq_t base;
            if (m == 1) {
                base = (seq_t)1;             /* any value works, use 1 */
            } else {
                /* search for smallest b ≥ 2 with sum ≡ 0 (mod m) */
                for (idx_t b = 2; ; ++b) {
                    idx_t cur  = 1 % m;
                    idx_t sum  = 1 % m;        /* 1 + b      */
                    for (idx_t k = 1; k < m; ++k) {
                        cur = (cur * b) % m;
                        sum = (sum + cur) % m;
                    }
                    if (sum % m == 0) {
                        base = (seq_t)b;
                        break;
                    }
                }
            }
            new_bases[m] = base;
        }

        bases      = new_bases;
        cache_cap  = r;
    }

    seq_t base = bases[r];

    /* compute term = base^pos */
    seq_t term = (seq_t)1;
    for (idx_t i = 0; i < pos; ++i)
        term = term * base;

    return term;
}
