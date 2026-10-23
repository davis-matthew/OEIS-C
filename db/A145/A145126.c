/*
 * A145126  a(n) = 1 + (6 + (11 + (6 + n)*n)*n)*n/24.
 * https://oeis.org/A145126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+3,4)+1
 * Range:     u8 n <= 7, u16 n <= 33, u32 n <= 565, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t gcd_seq(seq_t a, seq_t b) {
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t res = 1;
    seq_t N   = (seq_t)n;

    for (idx_t k = 1; k <= 4; ++k) {
        seq_t num = N + (k - 1);
        seq_t den = (seq_t)k;

        /* cancel common factors between res and den */
        seq_t g1 = gcd_seq(res, den);
        res /= g1;
        den /= g1;

        /* cancel remaining factors between num and den */
        seq_t g2 = gcd_seq(num, den);
        num /= g2;
        den /= g2;

        /* after cancellations den should be 1 */
        res *= num;
    }

    return res + 1;
}
