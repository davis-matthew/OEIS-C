/*
 * A227132  Numbers k for which there is an odd number of nonzero digits when
 *          k is written in the factorial base (A007623).
 * https://oeis.org/A227132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer k (k ≥ 1) whose factorial‑base
 *            representation has an odd number of non‑zero digits
 * Range:     u8 n <= 128, u16 n <= 2520, u32 n <= 2520, u64 n <= 2520,
 *            u128 n <= 2520, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Check whether the factorial‑base representation of x contains an odd number of non‑zero digits. */
static bool odd_nonzero_digits(idx_t x)
{
    unsigned nonzero = 0;
    idx_t i = 1;
    while (x > 0)
    {
        idx_t base = i + 1;
        idx_t d = x % base;
        if (d != 0u) ++nonzero;
        x /= base;
        ++i;
    }
    return (nonzero & 1u) != 0u;
}

/* a(n) */
seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 1 */
    if (n == 0) return 0;  /* undefined, caller should use n ≥ 1 */

    idx_t count = 0;
    idx_t k = 1;
    while (1)
    {
        if (odd_nonzero_digits(k))
        {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
