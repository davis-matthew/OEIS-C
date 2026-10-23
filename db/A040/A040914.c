/*
 * A040914  Continued fraction for sqrt(945).
 * https://oeis.org/A040914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction partial quotients for sqrt(945)
 * a(0) = 30
 * a(n) = period[(n-1) % 10] for n ≥ 1,
 * where period = [1, 2, 1, 6, 12, 6, 1, 2, 1, 60] */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)30;
    static const seq_t period[] = {
        (seq_t)1,
        (seq_t)2,
        (seq_t)1,
        (seq_t)6,
        (seq_t)12,
        (seq_t)6,
        (seq_t)1,
        (seq_t)2,
        (seq_t)1,
        (seq_t)60
    };
    return period[(n - 1) % 10];
}
