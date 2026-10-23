/*
 * A122504  a(n) = -a(n-6) + 3*a(n-5) + a(n-4) - 7*a(n-3) + a(n-2) + 3*a(n-1).
 * https://oeis.org/A122504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -a(n-6) + 3*a(n-5) + a(n-4) - 7*a(n-3) + a(n-2) +
 *            3*a(n-1). Initial values: a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=1
 * Range:     i8 n <= 11, i16 n <= 17, i32 n <= 31, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 6) return (seq_t)1;

    /* sliding window of the last six terms */
    seq_t t[6];
    for (int i = 0; i < 6; ++i) t[i] = (seq_t)1;
    seq_t current = 0;

    for (idx_t i = 7; i <= n; ++i) {
        current = -t[0] + (seq_t)3 * t[1] + t[2] - (seq_t)7 * t[3] + t[4] + (seq_t)3 * t[5];
        t[0] = t[1];
        t[1] = t[2];
        t[2] = t[3];
        t[3] = t[4];
        t[4] = t[5];
        t[5] = current;
    }
    return current;
}
