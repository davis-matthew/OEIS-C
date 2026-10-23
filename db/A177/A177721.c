/*
 * A177721  Number of line segments connecting exactly 5 points in an n x n
 *          grid of points.
 * https://oeis.org/A177721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{|dx|,|dy| ≤ n-1, gcd(|dx|,|dy|)=4} (n - |dx|)*(n -
 *            |dy|)/2
 * Range:     u8 n <= 10, u16 n <= 42, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*              = Sum_{a,b ≥ 0, (a,b)≠(0,0), gcd(a,b)=1, 4a+4b ≤ n-1}
 *                    w(a,b)*(n - 4a)*(n - 4b)
 *            where w(a,b)=2 if a>0 && b>0, w(a,b)=1 otherwise. */

static inline idx_t _gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n <= 0) return 0;
    /* Largest multiple of 4 that can appear as |dx| or |dy| */
    idx_t A = (n - 1) / 4;              /* floor((n-1)/4) */
    seq_t total = 0;

    for (idx_t a = 0; a <= A; ++a) {
        for (idx_t b = 0; b <= A; ++b) {
            if (a == 0 && b == 0) continue;
            if (_gcd_idx(a, b) != 1) continue;

            seq_t na = (seq_t)(n - 4 * a);
            seq_t nb = (seq_t)(n - 4 * b);
            seq_t term = na * nb;

            /* Weight according to whether both coordinates are non‑zero */
            if (a > 0 && b > 0) {
                total += (seq_t)2 * term;
            } else {
                total += term;
            }
        }
    }

    return total;
}
