/*
 * A031997  Odd numbers which when cubed give number composed just of the
 *          digits 0, 1, 2, 3.
 * https://oeis.org/A031997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A031997: odd numbers whose cube consists only of digits 0–3.
   For n=1 → 1,
   for n=7 → 684917,
   for 2≤n≤6 → 10^(n‑1)+1,
   for n≥8 → 10^(n‑2)+1. */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 7) return (seq_t)684917;
    if (n <= 6) return pow10(n - 1) + (seq_t)1;
    return pow10(n - 2) + (seq_t)1;
}
