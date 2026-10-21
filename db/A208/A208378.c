/*
 * A208378  Number of n X 7 0..1 arrays avoiding 0 0 0 and 1 1 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A208378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 42; a(n) = 84*n^3 + 840*n^2 - 1344*n + 420 for n > 1 (the
 *            recurrence a(n) = 4*a(n‑1) - 6*a(n‑2) + 4*a(n‑3) - a(n‑4) with
 *            initial values a(1)=42, a(2)=1764, a(3)=6216, a(4)=13860,
 *            a(5)=25200 is also satisfied for all n > 5)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used, but safe */
    if (n == 1) return (seq_t)42;

    seq_t nn = (seq_t)n;
    seq_t res = (seq_t)84 * nn * nn * nn;
    res += (seq_t)840 * nn * nn;
    res -= (seq_t)1344 * nn;
    res += (seq_t)420;
    return res;
}
