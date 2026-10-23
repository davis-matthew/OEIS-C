/*
 * A177505  Base 2i representation of n reinterpreted in base 4.
 * https://oeis.org/A177505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = Σ d_k * 16^k, where d_k are the digits of n in base –4.
 *            This follows from interpreting the quater‑imaginary (base 2i)
 *            digits as a base 16 number. The algorithm converts n to base –4
 *            by repeatedly extracting remainders modulo 4 and updating the
 *            signed intermediate value. It works for all n ≥ 0
 * Range:     u8 n <= 3, u16 n <= 51, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Signed intermediate.  n is small in tests (≤10 000),
       so 64‑bit signed is sufficient while keeping the code
       extremely fast.  For larger values a 128‑bit type could be
       used instead. */
    long long x = (long long)n;

    seq_t result = 0;
    seq_t pow16  = 1;          /* 16^k, starts with k = 0 */

    while (x != 0) {
        int r = (int)(x % 4);
        if (r < 0)            /* adjust for negative x */
            r += 4;

        result += (seq_t)r * pow16;   /* add digit * 16^k */
        pow16 *= 16;                  /* advance 16^k */

        x = (r - x) / 4;              /* next value for base –4 conversion */
    }

    return result;
}
