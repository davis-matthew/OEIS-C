/*
 * A021314  Expansion of 1/((1-x)*(1-2*x)*(1-10*x)*(1-11*x)).
 * https://oeis.org/A021314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*a(n-1) - 175*a(n-2) + 372*a(n-3) - 220*a(n-4) with
 *            a(0)=1, a(1)=24, a(2)=401, a(3)=5796
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)24;
    if (n == 2) return (seq_t)401;
    if (n == 3) return (seq_t)5796;

    /* rolling buffer to keep the last four computed values */
    seq_t buf[5] = {(seq_t)1, (seq_t)24, (seq_t)401, (seq_t)5796, (seq_t)0};

    for (idx_t i = 4; i <= n; ++i) {
        seq_t t1 = buf[(i - 1) % 5];
        seq_t t2 = buf[(i - 2) % 5];
        seq_t t3 = buf[(i - 3) % 5];
        seq_t t4 = buf[(i - 4) % 5];

        /* compute a(i) using the recurrence */
        seq_t next = (seq_t)24 * t1
                   - (seq_t)175 * t2
                   + (seq_t)372 * t3
                   - (seq_t)220 * t4;

        buf[i % 5] = next;
    }

    return buf[n % 5];
}
