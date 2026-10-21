/*
 * A064313  Integer part of area of a regular polygon with n sides each of
 *          length 1.
 * https://oeis.org/A064313
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/(4*tan(Pi/n)))
 * Range:     u8 none, u16 n <= 2, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) = floor((3*n^2*Q^2 - P^2)/(12*P*Q)) */
/* with π ≈ P/Q = 104348/33215, error < 3·10^−10. */

seq_t a(idx_t n) {
    if (n < 2) return 0;
    const seq_t P = 104348;
    const seq_t Q = 33215;

    unsigned long long nl = (unsigned long long)n;
    unsigned long long p  = (unsigned long long)P;
    unsigned long long q  = (unsigned long long)Q;

    unsigned long long numer = 3ULL * nl * nl * q * q - p * p;
    unsigned long long denom = 12ULL * p * q;

    return (seq_t)(numer / denom);
}
