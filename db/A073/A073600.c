/*
 * A073600  Least n^k with same leading decimal digit as n, k>1.
 * https://oeis.org/A073600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = n^k, k>1 minimal such that the leading decimal digit of
 *            n^k equals the leading decimal digit of n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 30,
 *            big any n
 * Helpers:   leading_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include "../utils/leading_digit.h"

seq_t a(idx_t n) {
    if (n == 1u) return (seq_t)1;   /* special case – 1^k is always 1 */

    /* leading digit of n (1 … 9) */
    uint64_t ld = leading_digit((uint64_t)n);

    /* log10(n) – used to compute leading digit of n^k via fractional part */
    long double log10_n = log10l((long double)n);

    int k = 2;                       /* search for the minimal k > 1 */
    for (;;) {
        long double f = fmodl((long double)k * log10_n, 1.0L); /* fractional part */
        if (f < 0.0L) f += 1.0L;        /* safety against rounding */

        long double pw = powl(10.0L, f);    /* 10^{fractional part} */
        int lead = (int)pw;                 /* integer part gives leading digit */
        if (lead == 10) lead = 9;           /* guard against rounding giving 10 */

        if ((uint64_t)lead == ld) break;    /* desired leading digit found */
        ++k;
    }

    /* compute n^k (may be large; seq_t may be arbitrary‑precision) */
    seq_t result = (seq_t)n;
    for (int i = 1; i < k; ++i) result *= (seq_t)n;
    return result;
}
