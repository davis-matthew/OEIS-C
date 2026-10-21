/*
 * A208601  Number of 7-bead necklaces labeled with numbers -n..n not allowing
 *          reversal, with sum zero.
 * https://oeis.org/A208601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 57, a(2) = 1163, a(3) = 8671, a(4) = 39019, a(5) =
 *            129823, a(6) = 353333, a(7) = 833253. For n > 7: a(n) = 7*a(n-1)
 *            - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5) - 7*a(n-6) +
 *            a(n-7)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 30, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial seven terms. */
    static const seq_t init[7] = {
        (seq_t)57,
        (seq_t)1163,
        (seq_t)8671,
        (seq_t)39019,
        (seq_t)129823,
        (seq_t)353333,
        (seq_t)833253
    };

    if (n <= 7)
        return init[(size_t)(n - 1)];

    /* Working buffer holding the last seven computed values. */
    seq_t buf[7];
    for (size_t i = 0; i < 7; ++i)
        buf[i] = init[i];

    for (idx_t k = 8; k <= n; ++k) {
        /* Compute next term using the recurrence without negative
           intermediates: sum positive & negative parts separately. */
        seq_t pos = buf[6] * (seq_t)7
                  + buf[4] * (seq_t)35
                  + buf[2] * (seq_t)21
                  + buf[0];
        seq_t neg = buf[5] * (seq_t)21
                  + buf[3] * (seq_t)35
                  + buf[1] * (seq_t)7;
        seq_t next = pos - neg;

        /* Shift buffer and insert new value. */
        for (size_t i = 0; i < 6; ++i)
            buf[i] = buf[i + 1];
        buf[6] = next;
    }

    return buf[6];
}
