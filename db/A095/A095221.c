/*
 * A095221  (Concatenation 1,2,3,...,n) mod n.
 * https://oeis.org/A095221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = Concatenation(1,2,…,n) mod n (A007908(n) mod n)
 * Range:     u8 n <= 282, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1 – the sequence is defined for n ≥ 1 */

    seq_t current = 0;
    seq_t modn    = (seq_t)n;      /* modulo value – fits in seq_t because n ≤ 2⁶⁴ */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* number of decimal digits of i */
        idx_t tmp = i;
        idx_t digits = 0;
        while (tmp)
        {
            tmp /= 10;
            ++digits;
        }

        /* compute 10^digits (mod n) using 128‑bit intermediate to avoid overflow */
        seq_t pow10 = 1;
        for (idx_t d = 0; d < digits; ++d)
        {
            pow10 = (seq_t)(((unsigned __int128)pow10 * 10) % modn);
        }

        /* update current = (current * 10^digits + i) mod n */
        seq_t i_mod = (seq_t)(i % n);
        current = (seq_t)(((unsigned __int128)current * pow10 + i_mod) % modn);
    }

    return current;
}
