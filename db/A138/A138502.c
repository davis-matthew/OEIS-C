/*
 * A138502  Expansion of q^(-1/2) * (eta(q)^4 * eta(q^4)^2 / eta(q^2)^3)^2 in
 *          powers of q.
 * https://oeis.org/A138502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = b(2*n + 1) where b(i) is multiplicative on odd integers:
 *            b(1) = 1 for an odd prime p and exponent e >= 1 if p ≡ 1 (mod
 *            4): b(p^e) = (p^(2(e+1)) - 1) / (p^2 - 1) if p ≡ 3 (mod 4):
 *            b(p^e) = (1 - (-p^2)^(e+1)) / (p^2 + 1) This exactly yields the
 *            OEIS A138502
 * Range:     i8 n <= 1, i16 n <= 7, i32 n <= 342, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i) result *= base;
    return result;
}

seq_t a(idx_t n) {
    idx_t m = 2 * n + 1;          /* odd argument */
    seq_t result = 1;

    idx_t temp = m;
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if ((temp % p) != 0) continue;
        idx_t exp = 0;
        while ((temp % p) == 0) {
            temp /= p;
            ++exp;
        }

        seq_t p2 = (seq_t)p * (seq_t)p;
        seq_t contrib;
        if (p % 4 == 1) {
            seq_t numerator = pow_seq(p2, exp + 1) - 1;
            seq_t denominator = p2 - 1;
            contrib = numerator / denominator;
        } else { /* p % 4 == 3 */
            seq_t p2pow = pow_seq(p2, exp + 1);
            if ((exp + 1) % 2 == 1) p2pow = -p2pow; /* (-p^2)^(exp+1) */
            seq_t numerator = 1 - p2pow;
            seq_t denominator = p2 + 1;
            contrib = numerator / denominator;
        }
        result *= contrib;
    }
    if (temp > 1) { /* remaining prime factor */
        seq_t p = temp;
        seq_t p2 = (seq_t)p * (seq_t)p;
        seq_t contrib;
        if (p % 4 == 1) {
            seq_t numerator = pow_seq(p2, 2) - 1;  /* exp = 1 -> exp+1 = 2 */
            seq_t denominator = p2 - 1;
            contrib = numerator / denominator;
        } else { /* p % 4 == 3 */
            seq_t p2pow = pow_seq(p2, 2); /* (-p^2)^2 = p^4 */
            /* even exponent, no sign change */
            seq_t numerator = 1 - p2pow;
            seq_t denominator = p2 + 1;
            contrib = numerator / denominator;
        }
        result *= contrib;
    }

    return result;
}
