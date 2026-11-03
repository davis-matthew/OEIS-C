/*
 * A034048  Numbers with multiplicative digital root value 0.
 * https://oeis.org/A034048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th integer (starting from 0) whose * multiplicative
 *            digital root is 0. The algorithm scans consecutive * integers
 *            and tests each for that property
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9874 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Test whether the multiplicative digital root of the
 * non‑negative integer x (held in idx_t) is zero. */
static bool has_mdr_zero(idx_t x)
{
    while (x >= 10) {
        idx_t prod = 1;
        idx_t d = x;
        while (d > 0) {
            prod *= d % 10;
            if (prod == 0) return true;      /* zero digit encountered */
            d /= 10;
        }
        /* If the product itself is zero, the root will be zero. */
        if (prod == 0) return true;
        x = prod;                            /* continue with product */
    }
    return x == 0;                            /* final single digit */
}

/* Return the n‑th term of A034048 (1‑based index). */
seq_t a(idx_t n)
{
    idx_t found = 0;            /* number of qualifying integers seen */
    idx_t x = 0;                /* current integer being tested */

    while (found < n) {
        if (has_mdr_zero(x)) {
            ++found;
            if (found == n)
                return (seq_t)x;   /* cast to the output type                      */
        }
        ++x;
    }
    /* Unreachable for n >= 1, but return 0 to satisfy the compiler. */
    return (seq_t)0;
}
