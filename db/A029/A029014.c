/*
 * A029014  Expansion of 1/((1-x)*(1-x^2)*(1-x^5)*(1-x^9)).
 * https://oeis.org/A029014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1, 2, 5, and 9.
 *            Recurrence (valid for n >= 17): a(n) =
 *            a(n-1)+a(n-2)-a(n-3)+a(n-5)-a(n-6)-a(n-7)+a(n-8)+a(n-9)
 *            -a(n-10)-a(n-11)+a(n-12)-a(n-14)+a(n-15)+a(n-16)-a(n-17), with
 *            initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=2, a(4)=3, a(5)=4,
 *            a(6)=5, a(7)=6,...
 * Range:     u8 n <= 43, u16 n <= 319, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base values for n = 0..16.  Cast to seq_t to handle arbitrary types. */
    static const seq_t base[17] = {
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)2, (seq_t)3,
        (seq_t)4, (seq_t)5, (seq_t)6, (seq_t)7, (seq_t)9,
        (seq_t)11, (seq_t)13, (seq_t)15, (seq_t)17, (seq_t)20,
        (seq_t)23, (seq_t)26
    };

    if (n <= 16)
        return base[n];

    /* Circular buffer that always holds the last 17 values:
       buf[0] = a(k-17), buf[1] = a(k-16), … , buf[16] = a(k-1)   */
    seq_t buf[17];
    for (int i = 0; i < 17; ++i)
        buf[i] = base[i];

    for (idx_t k = 17; k <= n; ++k) {
        /* Compute a(k) from the recurrence using only additions/subtractions
           that will not underflow: sum of positive terms minus sum of
           negative terms. */
        seq_t pos = buf[16];  /* a(k-1) */
        pos += buf[15];       /* a(k-2) */
        pos += buf[12];       /* a(k-5) */
        pos += buf[9];        /* a(k-8) */
        pos += buf[8];        /* a(k-9) */
        pos += buf[5];        /* a(k-12) */
        pos += buf[2];        /* a(k-15) */
        pos += buf[1];        /* a(k-16) */

        seq_t neg = buf[14];  /* a(k-3) */
        neg += buf[11];       /* a(k-6) */
        neg += buf[10];       /* a(k-7) */
        neg += buf[7];        /* a(k-10) */
        neg += buf[6];        /* a(k-11) */
        neg += buf[3];        /* a(k-14) */
        neg += buf[0];        /* a(k-17) */

        seq_t next = pos - neg;

        /* Slide the buffer: drop a(k-17) and add a(k) at the end. */
        for (int i = 0; i < 16; ++i)
            buf[i] = buf[i + 1];
        buf[16] = next;
    }

    /* After the loop, buf[16] holds a(n). */
    return buf[16];
}
