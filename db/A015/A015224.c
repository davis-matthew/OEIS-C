/*
 * A015224  Even pentagonal pyramidal numbers.
 * https://oeis.org/A015224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-3) - 3*a(n-6) + a(n-9) + 192 (n ≥ 9) Initial
 *            values: a(0)=0, a(1)=6, a(2)=18, a(3)=40, a(4)=126, a(5)=196,
 *            a(6)=288, a(7)=550, a(8)=726 (These match the OEIS sequence
 *            A015224.)
 * Range:     u8 n <= 5, u16 n <= 37, u32 n <= 1535, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* first nine terms */
    static const seq_t base[9] = {
        (seq_t)0ULL,
        (seq_t)6ULL,
        (seq_t)18ULL,
        (seq_t)40ULL,
        (seq_t)126ULL,
        (seq_t)196ULL,
        (seq_t)288ULL,
        (seq_t)550ULL,
        (seq_t)726ULL
    };

    /* for indices below 9 return the predefined values */
    if (n < 9ULL) {
        return base[(size_t)n];
    }

    /* circular buffer holding the last ten computed terms
       buf[i % 10] holds a(i).  For 0 ≤ i < 9 initialised from base. */
    seq_t buf[10];
    for (idx_t i = 0; i < 9; ++i) {
        buf[i] = base[(size_t)i];
    }

    /* compute all terms up to n using the recurrence */
    for (idx_t i = 9; i <= n; ++i) {
        seq_t t1 = (seq_t)3 * buf[(i - 3) % 10];
        seq_t t2 = (seq_t)3 * buf[(i - 6) % 10];
        seq_t t3 = buf[(i - 9) % 10];
        buf[i % 10] = t1 - t2 + t3 + (seq_t)192ULL;
    }

    return buf[n % 10];
}
