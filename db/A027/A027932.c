/*
 * A027932  T(n, 2n-9), T given by A027926.
 * https://oeis.org/A027932
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..4} binomial(n-k, 9-2*k) for n ≥ 5
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 52, u64 n <= 577,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative binomial using 128‑bit intermediate to avoid overflow
 * (works for uint64_t, uint8_t, uint16_t, uint32_t, uint64_t, unsigned __int128
 * and for arbitrary‑precision seq_t classes that support the usual operators). */
static seq_t binom_unsigned(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;            /* use symmetry */

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        idx_t num = n - k + i;           /* numerator part */
        idx_t den = i;                   /* denominator part */

        /* 128‑bit intermediate guarantees no overflow for 64‑bit seq_t */
        res = ( (unsigned __int128)res * (unsigned __int128)num ) / den;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 5) return 0;                /* offset 5: undefined for n<5 */

    seq_t sum = 0;
    for (idx_t k = 0; k < 5; ++k) {
        idx_t N = n - k;                /* n - k */
        idx_t K = 9 - 2 * k;            /* 9 - 2*k */
        sum += binom_unsigned(N, K);
    }
    return sum;
}
