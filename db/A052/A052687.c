/*
 * A052687  Expansion of e.g.f. (1+x-x^3)/((1-x)*(1-x^2)).
 * https://oeis.org/A052687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * (2*n + 7 + (-1)^n) / 4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static idx_t max_n = 0;
    static seq_t fact = (seq_t)1;

    if (n == 0) return (seq_t)1;

    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            fact = fact * (seq_t)i;
        }
        max_n = n;
    } else if (n < max_n) {
        fact = (seq_t)1;
        for (idx_t i = 1; i <= n; ++i) {
            fact = fact * (seq_t)i;
        }
        max_n = n;
    }

    int64_t factorInt = (int64_t)2 * (int64_t)n + 7 + ((n % 2 == 0) ? 1 : -1);
    seq_t result = fact * (seq_t)factorInt;
    result = result / (seq_t)4;
    return result;
}
