/*
 * A182929  The rows of the binomial triangle reduced to balanced ternary
 *          lists encoded as decimal numbers.
 * https://oeis.org/A182929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} [binomial(n-1,k) ~ n] * 3^k in balanced
 *            ternary
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;          /* a(1) is pre‑determined */

    seq_t result = 0;
    seq_t pow3   = 1;              /* 3^k, updated each iteration   */
    seq_t C      = 1;              /* current binomial coefficient   */
    idx_t m = n;

    for (idx_t k = 0; k < n; ++k) {
        /* signed remainder of C modulo n */
        idx_t mod   = (idx_t)(C % (seq_t)m);
        int64_t rem = (int64_t)mod;
        if (rem > (int64_t)(m / 2))
            rem -= (int64_t)m;

        if (rem == 1)        result += pow3;
        else if (rem == -1)  result -= pow3;

        if (k + 1 < n) {                       /* prepare next coefficient */
            C = (C * (seq_t)(n - 1 - k)) / (seq_t)(k + 1);
        }

        pow3 *= 3;
    }
    return result;
}
