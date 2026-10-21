/*
 * A211681  Numbers such that all the substrings of length <= 2 are primes.
 * https://oeis.org/A211681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   For n ≥ 1, let r = (n−1) mod 8 and k = floor((n−1)/8). Then a(n)
 *            = base[r]·10^(2k) + offset[r]·(10^(2k)–1)/99 with base =
 *            {2,3,5,7,23,37,53,73} and offset = {37,73,37,37,73,37,73,73}
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 38, u64 n <= 76, u128 n <= 153,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1, return 0 */

    /* Determine k and remainder r (0‑7) */
    idx_t k = (n - 1) / 8;
    idx_t r = (n - 1) % 8;

    /* Constants for each residue class */
    static const seq_t base[8]   = {2, 3, 5, 7, 23, 37, 53, 73};
    static const seq_t offset[8] = {37,73,37,37,73,37,73,73};

    /* Compute 10^(2k) */
    seq_t t = pow10(2 * k);          /* 10^(2k) */

    /* (10^(2k) – 1) / 99 is integer because 10^m – 1 is divisible by 9 and 11 */
    seq_t part = (t - 1) / 99;

    /* Result using the derived formula */
    return base[r] * t + offset[r] * part;
}
