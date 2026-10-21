/*
 * A231001  Number of years after which an entire year can have the same
 *          calendar, in the Julian calendar.
 * https://oeis.org/A231001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-5) - a(n-6) for n > 5; Initial values:
 *            a(0)=0, a(1)=6, a(2)=11, a(3)=17, a(4)=22, a(5)=28
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first six terms directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)17;
    if (n == 4) return (seq_t)22;
    if (n == 5) return (seq_t)28;

    /* Use a sliding window of the last six values. */
    seq_t v[6] = {(seq_t)0, (seq_t)6, (seq_t)11, (seq_t)17, (seq_t)22, (seq_t)28};

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = v[5] + v[1] - v[0];   /* a(i-1) + a(i-5) - a(i-6) */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = next;
    }

    return v[5];
}
