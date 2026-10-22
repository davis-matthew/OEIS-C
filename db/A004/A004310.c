/*
 * A004310  Binomial coefficient C(2n,n-4).
 * https://oeis.org/A004310
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(2n, n-4), computed multiplicatively as C(2n,k) =
 *            C(2n,k-1)*(2n-k+1)/k with gcd reduction
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t gcd_s(seq_t x, seq_t y) {
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t two_n = (seq_t)n * 2;
    idx_t k = n - 4;
    idx_t i;

    for (i = 0; i < k; i++) {
        seq_t num = two_n - (seq_t)i;
        seq_t den = (seq_t)i + 1;
        seq_t g = gcd_s(num, den);
        num /= g;
        den /= g;
        /* result*num is divisible by den and gcd(num,den)=1, so den | result */
        result = (result / den) * num;
    }

    return result;
}
