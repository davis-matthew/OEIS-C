/*
 * A226579  Smallest number of integer-sided squares needed to tile a 6 X n
 *          rectangle.
 * https://oeis.org/A226579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + a(n-6) for n > 7, with a(0)=0, a(1)=6, a(2)=3,
 *            a(3)=2, a(4)=3, a(5)=5, a(6)=1, a(7)=5
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t base[8] = {
        (seq_t)0, /* a(0) */
        (seq_t)6, /* a(1) */
        (seq_t)3, /* a(2) */
        (seq_t)2, /* a(3) */
        (seq_t)3, /* a(4) */
        (seq_t)5, /* a(5) */
        (seq_t)1, /* a(6) */
        (seq_t)5  /* a(7) */
    };

    if (n <= 7)
        return base[(size_t)n];

    /* For n > 7, a(n) = 1 + a(n-6).  Compute how many 6‑steps back
       are required to reach an index ≤ 7, and add that number of
       increments. */
    idx_t k   = (n - 8) / 6 + 1;          /* number of 6‑step increments */
    idx_t idx = n - 6 * k;                /* index now in 0…7 */

    /* idx will be 0…7; the addition is performed in seq_t. */
    return (seq_t)k + base[(size_t)idx];
}
