/*
 * A059507  Transform of A059502 applied to sequence 4,5,6,...
 * https://oeis.org/A059507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3) - a(n-4), a(1)=4, a(2)=9,
 *            a(3)=24, a(4)=66
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 0) return 0;   /* value below offset is unspecified */

    /* initial terms */
    switch (n) {
    case 1: return (seq_t)4;
    case 2: return (seq_t)9;
    case 3: return (seq_t)24;
    case 4: return (seq_t)66;
    }

    /* iterative computation using the recurrence */
    seq_t a0 = (seq_t)4;  /* a(n-4) */
    seq_t a1 = (seq_t)9;  /* a(n-3) */
    seq_t a2 = (seq_t)24; /* a(n-2) */
    seq_t a3 = (seq_t)66; /* a(n-1) */

    idx_t i;
    for (i = 5; i <= n; ++i) {
        seq_t ai = 6 * a3 - 11 * a2 + 6 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
