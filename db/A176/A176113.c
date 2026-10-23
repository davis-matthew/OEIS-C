/*
 * A176113  Determinant of n X n matrix whose (i,j)-th element is (i+j)^(n-1).
 * https://oeis.org/A176113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^floor(n/2) * ((n-1)!)^n
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 5, i64 n <= 6, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n ≥ 1, with a(0) defined as 0 for safety. */

static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safe guard for non‑positive indices */

    /* sign = (-1)^floor(n/2) */
    seq_t sign = (((n >> 1) & 1) ? (seq_t)(-1) : (seq_t)(1));

    /* compute (n-1)! */
    seq_t fact = 1;
    for (idx_t i = 2; i < n; ++i) {
        fact = fact * (seq_t)i;
    }

    /* compute fact^n */
    seq_t power = pow_seq_t(fact, n);

    return sign * power;
}
