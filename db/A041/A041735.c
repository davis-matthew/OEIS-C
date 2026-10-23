/*
 * A041735  Denominators of continued fraction convergents to sqrt(387).
 * https://oeis.org/A041735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 6964*a(n-6) - a(n-12) for n>11 Initial values: a(0)=1,
 *            a(1)=1, a(2)=3, a(3)=58, a(4)=119, a(5)=177, a(6)=6845,
 *            a(7)=7022, a(8)=20889, a(9)=403913, a(10)=828715, a(11)=1232628
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 12 initial terms of the sequence */
    static const seq_t init[12] = { (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)58,
                                    (seq_t)119, (seq_t)177, (seq_t)6845,
                                    (seq_t)7022, (seq_t)20889, (seq_t)403913,
                                    (seq_t)828715, (seq_t)1232628 };

    /* For n < 12 just return the stored initial value */
    if (n < 12)
        return init[ (size_t)n ];

    /* Working buffer of 12 successive terms, used cyclically */
    seq_t buf[12];
    for (size_t i = 0; i < 12; ++i)
        buf[i] = init[i];

    /* Compute terms from 12 up to n */
    for (size_t i = 12; i <= (size_t)n; ++i)
    {
        /* recurrence: a(i) = 6964*a(i-6) - a(i-12) */
        seq_t next = (seq_t)6964 * buf[(i - 6) % 12] - buf[(i - 12) % 12];
        buf[i % 12] = next;
    }

    /* Result is stored at position corresponding to n */
    return buf[ (size_t)n % 12 ];
}
