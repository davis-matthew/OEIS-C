/*
 * A041557  Denominators of continued fraction convergents to sqrt(296).
 * https://oeis.org/A041557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0) = 1, a(1) = 4, a(2) = 5, a(3) = 39, a(4) = 44, a(5) = 215,
 *            a(6) = 7354, a(7) = 29631, a(8) = 36985, a(9) = 288526, a(10) =
 *            325511, a(11) = 1590570; a(n) = 7398*a(n-6) - a(n-12) for n > 11
 *            (denominators of the convergents to sqrt(296))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial 12 terms */
    static const seq_t init[12] = {
        (seq_t)1, (seq_t)4,  (seq_t)5,  (seq_t)39,
        (seq_t)44, (seq_t)215, (seq_t)7354,
        (seq_t)29631, (seq_t)36985, (seq_t)288526,
        (seq_t)325511, (seq_t)1590570
    };

    if (n < 12)
        return init[(size_t)n];

    /* circular buffer containing the last 12 computed values */
    seq_t buf[12];
    for (size_t k = 0; k < 12; ++k)
        buf[k] = init[k];

    for (idx_t i = 12; i <= n; ++i) {
        size_t idx6  = (size_t)((i - 6) % 12);
        size_t idx12 = (size_t)((i - 12) % 12);
        seq_t val = (seq_t)7398 * buf[idx6] - buf[idx12];
        buf[(size_t)(i % 12)] = val;
    }

    return buf[(size_t)(n % 12)];
}
