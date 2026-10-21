/*
 * A068219  Denominators of coefficients in log(1+x)*(1+x)^(1/3) power series.
 * https://oeis.org/A068219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = 3*n if n ≡ 1 or 3 (mod 4) * 3*n/2 if n ≡ 2 (mod 4) *
 *            3*n/4 if n ≡ 0 (mod 4)
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* sequence is defined for n >= 1 */
    seq_t v = (seq_t)n;
    seq_t prod = v * ((seq_t)3);          /* compute 3*n */
    switch (n & 3) {                      /* n % 4 */
        case 1:
        case 3:
            return prod;                  /* 3*n */
        case 2:
            return prod / 2;              /* 3*n/2 */
        case 0:
            return prod / 4;              /* 3*n/4 */
    }
    return 0; /* unreachable */
}
