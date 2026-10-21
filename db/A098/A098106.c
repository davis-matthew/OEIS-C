/*
 * A098106  Hankel transform of sequence (b(n)) where b(n) = Sum_{i=0..n}
 *          binomial(2*i,i).
 * https://oeis.org/A098106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(3k) = (-8)^k, * a(3k+1) = 3*(-8)^k*(2k+1), * a(3k+2) =
 *            6*(-8)^k*(2k+1)
 * Range:     i8 n <= 4, i16 n <= 12, i32 n <= 27, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (-8)^m where m = n/3 */
    idx_t  m = n / 3;
    idx_t  r = n % 3;

    /* compute 8^m by binary exponentiation */
    seq_t pow_val = 1;
    seq_t base    = 8;
    idx_t  e      = m;
    while (e) {
        if (e & 1U)
            pow_val *= base;
        base *= base;
        e >>= 1U;
    }
    /* apply the minus sign if m is odd */
    if (m & 1U)
        pow_val = -pow_val;

    /* handle the three residue classes */
    switch (r) {
        case 0U:
            return pow_val;
        case 1U: {
            seq_t mult   = 3 * pow_val;
            seq_t factor = 2 * ((seq_t)m) + 1;
            return mult * factor;
        }
        case 2U: {
            seq_t mult   = 6 * pow_val;
            seq_t factor = 2 * ((seq_t)m) + 1;
            return mult * factor;
        }
    }
    /* unreachable */
    return 0;
}
