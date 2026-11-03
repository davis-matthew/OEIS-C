/*
 * A173772  a(n) = (4*10^n + 23)/9.
 * https://oeis.org/A173772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4·10^n + 23)/9 (recurrence: a(1)=7, a(n)=10·a(n-1)−23
 *            for n>1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Undefined for n < 1 – return 0 */
    if (n == 0)
        return (seq_t)0;

    /* Compute directly using the recurrence to avoid any dynamic allocation. */
    seq_t val = (seq_t)7;                /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
        val = val * (seq_t)10 - (seq_t)23;   /* a(i)=10*a(i-1)-23 */

    return val;
}
