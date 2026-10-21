/*
 * A070140  Number of acute integer triangles with perimeter n having integral
 *          area.
 * https://oeis.org/A070140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of acute integer triangles with perimeter n *
 *            having integral area
 * Range:     u8 n <= 9, u16 n <= 41, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root for arbitrary width seq_t. */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    seq_t ans = 0;
    while (low <= high) {
        seq_t mid = low + ((high - low) >> 1);
        seq_t sq  = mid * mid;
        if (sq == x)
            return mid;
        if (sq < x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;  /* avoid underflow */
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t count = 0;

    /* Enumerate integer side lengths a <= b <= c with a + b + c = n */
    for (idx_t a = 1; a <= n / 3; ++a) {
        idx_t b_start = a;
        idx_t b_end   = (n - a) / 2;   /* ensures b <= c */
        for (idx_t b = b_start; b <= b_end; ++b) {
            idx_t c = n - a - b;
            /* Triangle inequality is satisfied automatically by loop bounds. */

            /* Acute-ness: a² + b² > c² */
            seq_t asq = (seq_t)a * a;
            seq_t bsq = (seq_t)b * b;
            seq_t csq = (seq_t)c * c;
            if (asq + bsq <= csq) continue;

            /* Heron’s formula: 16·area² = (a+b+c)(-a+b+c)(a-b+c)(a+b-c) */
            seq_t d1 = (seq_t)a + (seq_t)b + (seq_t)c;
            seq_t d2 = (seq_t)b + (seq_t)c - (seq_t)a;
            seq_t d3 = (seq_t)c + (seq_t)a - (seq_t)b;
            seq_t d4 = (seq_t)a + (seq_t)b - (seq_t)c;

            seq_t D = d1 * d2 * d3 * d4;
            if ((D & (seq_t)15) != 0) continue;  /* must be divisible by 16 */

            seq_t t = D >> 4;  /* t = area² */

            /* Perfect‑square test of t */
            seq_t sqrt_t = floor_sqrt(t);
            if (sqrt_t * sqrt_t != t) continue;

            ++count;  /* valid acute triangle with integer area */
        }
    }
    return count;
}
