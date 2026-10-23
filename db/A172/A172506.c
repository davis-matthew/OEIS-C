/*
 * A172506  a(n) = numerator of fraction a/b, where gcd(a, b) = 1, whose
 *          decimal representation has the form
 *          (1)(2)(3)...(n-1)(n).(1)(2)(3)...(n-1)(n).
 * https://oeis.org/A172506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (C_n * (10^d + 1)) / gcd(C_n, 10^d) where C_n =
 *            concatenation of 1,2,...,n and d = digit length of C_n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline int digit_count_idx(idx_t x) {
    int d = 0;
    do { ++d; x/=10; } while (x);
    return d;
}

static inline const seq_t pow10digit[6] = {1,10,100,1000,10000,100000};

static inline seq_t seq_gcd(seq_t a, seq_t b) {
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* safety, though n>=1 in OEIS */

    /* Build C_n and count its digits */
    seq_t C = 1;
    int total_digits = 1;                 /* digits of 1 */

    for (idx_t k = 2; k <= n; ++k) {
        int dk = digit_count_idx(k);     /* digits of k, <=5 for k<=10000 */
        C = C * pow10digit[dk] + k;
        total_digits += dk;
    }

    /* Compute 10^d */
    seq_t pow10len = 1;
    for (idx_t i = 0; i < (idx_t)total_digits; ++i)
        pow10len *= (seq_t)10;

    /* Numerator before reduction: C * 10^d + C */
    seq_t numerator_raw = C * pow10len + C;

    /* Reduce by gcd(C,10^d) */
    seq_t g = seq_gcd(C, pow10len);
    return numerator_raw / g;
}
