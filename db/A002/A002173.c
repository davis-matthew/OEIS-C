/*
 * A002173  a(n) = Sum_{d|n, d == 1 mod 4} d^2 - Sum_{d|n, d == 3 mod 4} d^2.
 * https://oeis.org/A002173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult, look
 * Formula:   multiplicative with a(2^e) = 1, a(p^e) = Sum_{k=0..e} (p^2)^k
 *            for p == 1 (mod 4), a(p^e) = Sum_{k=0..e} (-p^2)^k for p == 3
 *            (mod 4)
 * Range:     i8 n <= 12, i16 n <= 184, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9773 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t a002173_local(idx_t p, idx_t e) {
    seq_t q = (seq_t)p * (seq_t)p;
    seq_t s = 1;
    idx_t k;
    if (p % 4 == 3) q = -q;
    for (k = 0; k < e; k++)
        s = 1 + q * s;
    return s;
}

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t temp = n;
    idx_t p;

    while (temp % 2 == 0) temp /= 2;

    for (p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                e++;
                temp /= p;
            }
            result *= a002173_local(p, e);
        }
    }
    if (temp > 1)
        result *= a002173_local(temp, 1);

    return result;
}
