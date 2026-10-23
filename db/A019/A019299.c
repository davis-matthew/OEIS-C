/*
 * A019299  First n elements of Thue-Morse sequence A010059 read as a binary
 *          number.
 * https://oeis.org/A019299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binary number formed by the first (n+1) bits of the *
 *            complement of the Thue–Morse sequence (A010059), with the *
 *            left‑most bit (i = 0) most significant. Equivalently, * a(n) =
 *            Σ_{i=0..n} (1 ^ parity(i)) · 2^(n-i). * (parity(i) = popcount(i)
 *            mod 2)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t i = 0; i <= n; ++i)
    {
        /* parity(i) = popcount(i) mod 2.  __builtin_parityll returns
         * this value for a 64‑bit integer.  Complement gives bit =
         * 1 if parity is 0 (even popcount), else 0. */
        int bit = 1 ^ __builtin_parityll((unsigned long long)i);
        res = (res << 1) | (seq_t)bit;
    }
    return res;
}
