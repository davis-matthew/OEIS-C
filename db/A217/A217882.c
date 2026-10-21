/*
 * A217882  Number of n element 0..2 arrays with each element the minimum of 7
 *          adjacent elements of a random 0..2 array of n+6 elements.
 * https://oeis.org/A217882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + 3*a(n-8) - a(n-9) +
 *            a(n-10) + a(n-11) + a(n-12) + a(n-13) + a(n-14) + a(n-15) for n
 *            > 15 Initial values (1 ≤ n ≤ 15): a(1)=3, a(2)=9, a(3)=22,
 *            a(4)=46, a(5)=86, a(6)=148, a(7)=239, a(8)=367, a(9)=546,
 *            a(10)=806, a(11)=1212, a(12)=1896, a(13)=3107, a(14)=5285,...
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 85, u128 n <= 174,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for n = 1..15, index 0 unused */
    static const seq_t BASE[16] = {
        0,
        (seq_t)3,  (seq_t)9,  (seq_t)22,  (seq_t)46,  (seq_t)86,
        (seq_t)148, (seq_t)239, (seq_t)367, (seq_t)546, (seq_t)806,
        (seq_t)1212, (seq_t)1896, (seq_t)3107, (seq_t)5285, (seq_t)9166
    };

    if (n == 0)                  /* outside the offset */
        return 0;

    /* Keep only the last 16 values in a circular buffer */
    seq_t buf[16];

    /* Initialise the first up to 15 terms directly from BASE */
    for (idx_t i = 1; i < 16 && i <= n; ++i)
        buf[i & 15] = BASE[i];

    if (n <= 15)                 /* already have the answer */
        return buf[n & 15];

    /* Compute terms from 16 up to n using the recurrence */
    for (idx_t i = 16; i <= n; ++i) {
        seq_t sum = buf[(i - 1) & 15] + buf[(i - 1) & 15] + buf[(i - 1) & 15]; /* 3*a(i-1) */
        sum += buf[(i - 3) & 15];                                           /* +a(i-3) */
        sum += buf[(i - 8) & 15] + buf[(i - 8) & 15] + buf[(i - 8) & 15];     /* +3*a(i-8) */
        sum += buf[(i -10) & 15] + buf[(i -11) & 15] + buf[(i -12) & 15]
              + buf[(i -13) & 15] + buf[(i -14) & 15] + buf[(i -15) & 15];   /* +a(i-10)..+a(i-15) */

        seq_t sub = buf[(i - 2) & 15] + buf[(i - 2) & 15] + buf[(i - 2) & 15]; /* 3*a(i-2) */
        sub += buf[(i - 9) & 15];                                           /* +a(i-9) */

        buf[i & 15] = sum - sub;
    }

    return buf[n & 15];
}
