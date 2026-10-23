/*
 * A049396  Expansion of (1-25*x)^(-8/5).
 * https://oeis.org/A049396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5^n / n!) * Π_{k=0}^{n-1} (5*k+8) Recurrence: a(0) = 1,
 *            a(n) = a(n-1) * (5*(n-1)+8) * 5 / n for n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        /* compute the numerator 5 * (5*(i-1)+8) = 25*i + 15 */
        unsigned __int128 term = (unsigned __int128)25 * i + 15;

        /* multiply using a 128‑bit accumulator to avoid overflow on
           64‑bit seq_t, then divide by i */
        unsigned __int128 tmp = (unsigned __int128)res * term;
        tmp /= i;

        res = (seq_t)tmp;
    }

    return res;
}
