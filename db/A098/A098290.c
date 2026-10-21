/*
 * A098290  Recurrence sequence based on positions of digits in decimal places
 *          of Zeta(3) (Apery's constant).
 * https://oeis.org/A098290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=0, a(1)=2, a(2)=1, a(3)=10, a(4)=208, a(5)=380, a(6)=394,
 *            a(7)=159; for n ≥ 8, a(n) = a(n‑5)
 * Range:     u8 n <= 4, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)1;

    static const seq_t cycle[5] = { (seq_t)10, (seq_t)208, (seq_t)380, (seq_t)394, (seq_t)159 };
    return cycle[(size_t)((n - 3) % 5)];
}
