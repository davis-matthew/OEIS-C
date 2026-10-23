/*
 * A027167  a(n) = Sum_{k=0..floor(n/2)} A027157(n-k, k).
 * https://oeis.org/A027167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} A027157(n-k, k) Equivalent
 *            recurrence (for n ≥ 7): a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3) +
 *            3*a(n-4) - 3*a(n-5) + 4*a(n-6) - 4*a(n-7) Initial terms: a(0)=1,
 *            a(1)=2, a(2)=4, a(3)=7, a(4)=15, a(5)=24, a(6)=50
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[7] = {
        (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)7,
        (seq_t)15, (seq_t)24, (seq_t)50
    };

    if (n < 7)
        return init[n];

    /* circular buffer for the last 7 values */
    seq_t buf[7];
    for (idx_t i = 0; i < 7; ++i)
        buf[i] = init[i];

    for (idx_t i = 7; i <= n; ++i) {
        /* compute positive contributions: a(n-1) + 2*a(n-2) + 3*a(n-4) + 4*a(n-6) */
        seq_t pos = buf[6];
        seq_t t = buf[5];
        t *= (seq_t)2;           /* 2*a(n-2) */
        pos += t;

        t = buf[3];
        t *= (seq_t)3;           /* 3*a(n-4) */
        pos += t;

        t = buf[1];
        t *= (seq_t)4;           /* 4*a(n-6) */
        pos += t;

        /* compute negative contributions: 2*a(n-3) + 3*a(n-5) + 4*a(n-7) */
        seq_t sub = buf[4];
        sub *= (seq_t)2;         /* 2*a(n-3) */

        t = buf[2];
        t *= (seq_t)3;           /* 3*a(n-5) */
        sub += t;

        t = buf[0];
        t *= (seq_t)4;           /* 4*a(n-7) */
        sub += t;

        /* next term = pos - sub  (pos > sub for all n) */
        seq_t next = pos - sub;

        /* shift buffer left by one */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = buf[6];
        buf[6] = next;
    }

    return buf[6];
}
