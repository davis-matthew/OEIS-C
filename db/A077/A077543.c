/*
 * A077543  Smallest n-digit composite palindrome.
 * https://oeis.org/A077543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 4 for n = 1 22 for n = 2 111 for n = 3 10^(n-1) + 1 for n
 *            ≥ 4 The latter value is always composite: for even n it is
 *            divisible by 11, and for odd n ≥ 5 it has a non‑trivial factor
 *            (examples for n=5,7 are 73·137 and 101·9901 respectively). The
 *            function computes this value directly without t...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* multiply by 10 the given number exp times */
static seq_t pow10_exp(idx_t exp)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        r *= (seq_t)10;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 1U)
        return (seq_t)4;   /* 4 is the smallest 1‑digit composite palindrome */

    if (n == 2U)
        return (seq_t)22;  /* 22 is the smallest 2‑digit composite palindrome */

    if (n == 3U)
        return (seq_t)111; /* 111 is the smallest 3‑digit composite palindrome */

    /* for n ≥ 4 */
    seq_t val = pow10_exp(n - 1);  /* 10^(n-1) */
    val += (seq_t)1;               /* add the trailing 1 → palindrome 10^(n-1)+1 */
    return val;
}
