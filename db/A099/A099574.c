/*
 * A099574  Diagonal sums of triangle A099573.
 * https://oeis.org/A099574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 32, u32 n <= 66, u64 n <= 135,
 *            u128 n <= 273, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  A099574 : Diagonal sums of triangle A099573
 *  Formula: a(n) = Σ_{k=0..⌊n/2⌋} Σ_{j=0..⌊k/2⌋} binomial(n-k-j, j)
 *  One convenient linear recurrence (derived from the rational generating
 *  function) is
 *      a(n) = a(n-1) + a(n-2) - a(n-3) + 2*a(n-4)
 *            - a(n-5) - a(n-6) - a(n-8)   for n ≥ 8
 *  Initial values (offset 0) are:
 *      a(0)=1, a(1)=1, a(2)=2, a(3)=2, a(4)=4,
 *      a(5)=5, a(6)=9, a(7)=11
 *
 *  The implementation uses only +, -, * as required and works for any
 *  seq_t type (primitive integer, uint128, or arbitrary‑precision class).
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base cases 0 … 7 */
    static const seq_t base[8] = { (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)2,
                                   (seq_t)4, (seq_t)5, (seq_t)9, (seq_t)11 };
    if (n < 8) return base[(size_t)n];

    /* Circular buffer of the last eight terms */
    seq_t buf[8];
    for (size_t i = 0; i < 8; ++i) buf[i] = base[i];

    seq_t result = 0;
    for (idx_t i = 8; i <= n; ++i)
    {
        seq_t a1 = buf[(i - 1) & 7];
        seq_t a2 = buf[(i - 2) & 7];
        seq_t a3 = buf[(i - 3) & 7];
        seq_t a4 = buf[(i - 4) & 7];
        seq_t a5 = buf[(i - 5) & 7];
        seq_t a6 = buf[(i - 6) & 7];
        seq_t a8 = buf[(i - 8) & 7];

        /* positive contributions: a1 + a2 + 2*a4 */
        seq_t pos = a1 + a2 + a4 + a4;
        /* negative contributions: a3 + a5 + a6 + a8 */
        seq_t neg = a3 + a5 + a6 + a8;

        result = pos - neg;
        buf[i & 7] = result;
    }
    return result;
}
