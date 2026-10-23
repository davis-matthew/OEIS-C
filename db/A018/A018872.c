/*
 * A018872  a(n)^5 is smallest fifth power beginning with n.
 * https://oeis.org/A018872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the smallest integer m such that m^5 starts * with the
 *            decimal representation of n. * This implementation uses a
 *            brute‑force search: for each n it * increments m until the
 *            leading digits of m^5 match n
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   leading_five_power_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "../utils/leading_five_power_digits.h"

/* ------------------------------------------------------------------ */
/* Helper: return the leading `need` decimal digits of m^5.             */

/* ------------------------------------------------------------------ */
/* Number of decimal digits of an unsigned integer.                     */
/* ------------------------------------------------------------------ */
static unsigned num_digits(idx_t x)
{
    unsigned d = 0;
    if (x == 0) return 1;
    while (x) {
        x /= 10;
        ++d;
    }
    return d;
}

/* ------------------------------------------------------------------ */
/* Main function required by the problem statement.                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Special case: 0^5 = 0 starts with 0. */
    if (n == 0) return (seq_t)0;

    unsigned need = num_digits(n);
    uint64_t m = 1;

    while (1) {
        seq_t lead = leading_five_power_digits(m, need);
        if (lead == n) return (seq_t)m;
        ++m;
    }
}
