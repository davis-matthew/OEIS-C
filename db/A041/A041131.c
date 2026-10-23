/*
 * A041131  Denominators of continued fraction convergents to sqrt(74).
 * https://oeis.org/A041131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 86*a(n-5) + a(n-10) with initial values a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=3, a(4)=5, a(5)=83, a(6)=88, a(7)=171, a(8)=259,
 *            a(9)=430
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 24, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial ten values.  Stored in a static constant array so they are
       available for all calls, including those with n < 10. */
    static const seq_t init[10] = {
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)5,
        (seq_t)83, (seq_t)88, (seq_t)171, (seq_t)259, (seq_t)430
    };

    /* If the requested index is within the pre‑known range, return it. */
    if (n < 10) {
        return init[(size_t)n];
    }

    /* Working buffer that holds the last ten values of the sequence.
       We initialise it with the first ten terms and then overwrite it
       using a ring‑buffer style update.  This uses only a constant
       amount of memory regardless of n. */
    seq_t buf[10];
    for (size_t i = 0; i < 10; ++i) {
        buf[i] = init[i];
    }

    seq_t next = 0; /* will hold a(i) for i >= 10 */
    for (idx_t i = 10; i <= n; ++i) {
        /* Indices of the two needed previous terms in the ring buffer. */
        idx_t idx_i_minus_5  = (i - 5) % 10;
        idx_t idx_i_minus_10 = (i - 10) % 10;

        next = buf[idx_i_minus_5] * (seq_t)86 + buf[idx_i_minus_10];
        /* Store the newly computed term back into the ring buffer. */
        buf[i % 10] = next;
    }

    return next;
}
