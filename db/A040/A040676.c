/*
 * A040676  Continued fraction for sqrt(703).
 * https://oeis.org/A040676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   partial quotients of the continued fraction of sqrt(703). a(0) =
 *            floor(sqrt(703)) = 26 For n ≥ 1, the terms repeat with period
 *            10: [1, 1, 17, 5, 1, 5, 17, 1, 1, 52]
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t period[20];
    static idx_t period_len = 0;

    if (period_len == 0) {
        /* generate the period once */
        const uint64_t N = 703;
        const uint64_t a0 = 26;          /* floor(sqrt(703)) */
        uint64_t m = 0, d = 1;
        uint64_t a = a0;

        /* generate partial quotients until 2*a0 is reached */
        do {
            m = d * a - m;
            d = (N - m * m) / d;
            a = (a0 + m) / d;
            period[period_len++] = (seq_t)a;
        } while (a != 2 * a0);
    }

    if (n == 0)
        return (seq_t)26;   /* integer part */

    idx_t idx = (n - 1) % period_len;
    return period[idx];
}
