/*
 * A053316  a(n) contains n digits (either '2' or '3') and is divisible by
 *          2^n.
 * https://oeis.org/A053316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = a(n-1) + 10^(n-1)*(2 + (a(n-1)/2^(n-1) mod 2)) Initial
 *            value: a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute a(n) for the OEIS sequence A053316 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0, return 0 for safety */
    seq_t a_val = 2;         /* a(1) */
    seq_t t      = 1;        /* a(1) / 2^1 */
    seq_t pow10  = 1;        /* 10^(k-1), will be updated each loop */
    seq_t pow5   = 1;        /* 5^(k-1), will be updated each loop */

    for (idx_t k = 2; k <= n; ++k) {
        pow10 *= 10;                /* 10^(k-1) */
        pow5  *= 5;                 /* 5^(k-1) */

        seq_t digit = 2 + (t & 1);  /* digit is 2 or 3 */
        a_val = a_val + pow10 * digit;
        t = (t + pow5 * digit) / 2; /* t = a(k)/2^k for next iteration */
    }
    return a_val;
}
