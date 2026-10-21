/*
 * A093027  Indices of terms in A093019 with value 7.
 * https://oeis.org/A093027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = nth positive integer k such that appending 7 to k yields
 *            a number with a valid Luhn mod‑10 checksum (i.e. Luhn(k*10+7) ≡
 *            0 (mod 10)). The sequence is obtained by searching for such k;
 *            there is no closed formula
 * Range:     u8 n <= 2, u16 n <= 1367, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   luhn_checksum_mod10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/luhn_checksum_mod10.h"

/* a(n) returns the n‑th index (1‑based) of A093019 whose term is 7. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset 1; undefined for n==0 */

    idx_t count = 0;
    idx_t candidate = 1;

    for (;; ++candidate)
    {
        seq_t combined = (seq_t)candidate * (seq_t)10 + (seq_t)7;
        if (luhn_checksum_mod10(combined) == 0)
        {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
    }
}
