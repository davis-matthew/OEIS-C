/*
 * A212966  Number of (w,x,y) with all terms in {0,...,n} and
 *          2*w=range{w,x,y}.
 * https://oeis.org/A212966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of triples (w,x,y) with w,x,y∈[0..n] and 2*w =
 *            max{w,x,y} - min{w,x,y}
 * Range:     u8 n <= 23, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t w_max = n >> 1;   /* floor(n/2) */

    for (idx_t w = 0; w <= w_max; ++w) {
        if (w == 0) {
            /* only triple (0,0,0) */
            sum += 1;
            continue;
        }

        /* term when w is the minimum (i.e., M = 3w ≤ n) */
        unsigned __int128 three_w = (unsigned __int128)w * 3;
        if (three_w <= n) {
            unsigned __int128 val = (unsigned __int128)w * 4 + 1;
            sum += (seq_t)val;
        }

        /* terms for all M with w strictly between min and max
           (i.e., M ∈ [2w, min(n, 3w‑1)] ) */
        idx_t low  = 2 * w;                    /* start of the interval */

        unsigned __int128 max_n_plus_one = (unsigned __int128)n + 1;
        idx_t high;
        if (three_w <= max_n_plus_one) {       /* 3w‑1 ≤ n */
            high = (idx_t)(three_w - 1);
        } else {                                /* 3w‑1 > n */
            high = n;
        }

        if (high >= low) {
            seq_t cnt = (seq_t)(high - low + 1);    /* number of interior M values */
            sum += cnt * 2;                        /* each contributes 2 triples */
        }
    }

    return sum;
}
