/*
 * A117963  Antidiagonal sums of a Legendre-binomial triangle for p = 3.
 * https://oeis.org/A117963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} Legendre( binom(n-k, k), 3 )
 * Range:     i8 n <= 2185, i16 n <= 8364, i32 n <= 8364, i64 n <= 8364,
 *            i128 n <= 8364, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Legendre(j,3) = 0 if j ≡ 0 (mod 3), 1 if j ≡ 1 (mod 3), -1 if j ≡ 2 (mod 3) */

static unsigned int binom_mod3(idx_t n, idx_t k)
{
    if (k > n) return 0;
    idx_t nn = n;
    idx_t kk = k;
    unsigned int res = 1;

    while (nn || kk) {
        unsigned int ni = (unsigned int)(nn % 3);
        unsigned int ki = (unsigned int)(kk % 3);
        if (ki > ni) return 0;

        unsigned int small;
        if (ki == 0 || ki == ni) {
            small = 1;
        } else if (ni == 2 && ki == 1) {
            small = 2;
        } else {
            small = 0; /* this case never occurs for ni ∈ {0,1,2} and ki∈{0,1,2} with ki>0<ni */
        }

        res = (res * small) % 3;
        nn /= 3;
        kk /= 3;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        unsigned int mod3 = binom_mod3(n - k, k);
        if (mod3 == 1) {
            sum += 1;
        } else if (mod3 == 2) { /* Legendre symbol is -1 */
            sum -= 1;
        } /* else mod3 == 0 contributes 0 */
    }
    return sum;
}
