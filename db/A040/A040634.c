/*
 * A040634  Continued fraction for sqrt(660).
 * https://oeis.org/A040634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=25; for n≥1 a(n)=period[(n-1)%6] with period {1,2,4,2,1,50}
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)25;

    static const seq_t period[6] = {
        (seq_t)1,
        (seq_t)2,
        (seq_t)4,
        (seq_t)2,
        (seq_t)1,
        (seq_t)50
    };

    idx_t idx = (n - 1) % 6;  /* n≥1 ensures n-1 is non‑negative */
    return period[idx];
}
