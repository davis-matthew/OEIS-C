/*
 * A069955  Let W(n) = Product_{k=1..n} (1 - 1/(4*k^2)), the partial Wallis
 *          product (lim_{n->oo} W(n) = 2/Pi); then a(n) = numerator(W(n)).
 * https://oeis.org/A069955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of W(n) where W(n) = ∏_{k=1..n} (1 – 1/(4k²))
 *            in lowest terms
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t num = 1;
    seq_t den = 1;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t tk = (seq_t)k;
        seq_t term_num = (seq_t)4 * tk * tk - 1;   /* 4k² – 1 */
        seq_t term_den = (seq_t)4 * tk * tk;       /* 4k²   */

        /* cross‑cancel new numerator against current denominator */
        seq_t g1 = gcd_seq(term_num, den);
        if (g1 != 1) {
            term_num /= g1;
            den /= g1;
        }

        /* cross‑cancel new denominator against current numerator */
        seq_t g2 = gcd_seq(term_den, num);
        if (g2 != 1) {
            term_den /= g2;
            num /= g2;
        }

        /* multiply the reduced terms */
        num *= term_num;
        den *= term_den;
    }

    /* after the loop the fraction num/den is in lowest terms; return the numerator */
    return num;
}
