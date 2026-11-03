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
 * Validated: standard, pass, 70 terms through n = 9331 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute Luhn checksum modulo 10 of a number given as seq_t.  The
   returned value is in the range 0–9. */
static inline unsigned luhn_checksum_mod10(seq_t x)
{
    seq_t sum = 0;
    bool double_flag = false;  /* rightmost digit has weight 1 */

    while (x > 0)
    {
        unsigned digit = (unsigned)(x % (seq_t)10);

        if (double_flag)
        {
            unsigned doubled = digit * 2;
            sum += (doubled > 9 ? doubled - 9 : doubled);
        }
        else
        {
            sum += digit;
        }

        double_flag = !double_flag;
        x /= (seq_t)10;
    }

    return (unsigned)(sum % 10);
}

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
