/*
 * A007914  Erroneous version of A048798.
 * https://oeis.org/A007914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = A048798(n) for n <= 27 and a(n) = A048798(n+1) for n >=
 *            28 (this erroneous version leaves out the term at index 28),
 *            where A048798(m) = Product p^((2*e) mod 3) over the prime powers
 *            p^e that divide m exactly
 * Range:     i8 n <= 12, i16 n <= 48, i32 n <= 48, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    for (idx_t i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

static seq_t a048798(idx_t m) {
    seq_t result = 1;
    idx_t exp = 0;
    while (m % 2 == 0) {
        exp++;
        m /= 2;
    }
    if (exp > 0) {
        result *= pow_seq((seq_t)2, (2 * exp) % 3);
    }
    for (idx_t p = 3; p * p <= m; p += 2) {
        exp = 0;
        while (m % p == 0) {
            exp++;
            m /= p;
        }
        if (exp > 0) {
            result *= pow_seq((seq_t)p, (2 * exp) % 3);
        }
    }
    if (m > 1) {
        result *= pow_seq((seq_t)m, 2);
    }
    return result;
}

seq_t a(idx_t n) {
    if (n >= 28) {
        return a048798(n + 1);
    }
    return a048798(n);
}
