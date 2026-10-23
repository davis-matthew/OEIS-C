/*
 * A025747  Index of 10^n within the sequence of the numbers of the form
 *          9^i*10^j.
 * https://oeis.org/A025747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{k=1..n} ceil(k * c) where c = log_9(10)
 * Range:     u8 n <= 21, u16 n <= 352, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Rational approximation of c = log_9(10) to 12 decimal places:
       c ≈ 1.04795163714 ≈ 1047951637140 / 1_000_000_000_000. */
    const unsigned long long P = 1047951637140ULL; /* numerator     */
    const unsigned long long Q = 1000000000000ULL; /* denominator   */

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        unsigned long long floor_k = (unsigned long long)
            (((unsigned __int128)k * P) / Q);      /* floor(k * c) */
        sum += (seq_t)floor_k + 1;                 /* ceil(k * c) */
    }
    return sum + 1; /* a(0) = 1 */
}
