/*
 * A040543  Continued fraction for sqrt(567).
 * https://oeis.org/A040543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0)=23; a(n)=period[(n-1) mod 6] with period=[1,4,3,4,1,46] for
 *            n>=1
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction for sqrt(567) */
    static const seq_t period[6] = {1, 4, 3, 4, 1, 46};

    if (n == 0) {
        return 23;
    }
    /* Since n>=1, n-1 is safe */
    return period[(n - 1) % 6];
}
