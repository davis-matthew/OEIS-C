/*
 * A093021  Indices of terms in A093019 with value 1.
 * https://oeis.org/A093021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 6553, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A093021:  indices of terms in A093019 with value 1.
    Compute by brute‑force search for the Luhn mod‑10 check digit being 1.
    For each integer m > 0, compute its Luhn check digit d(m).
    The nth integer with d(m) == 1 is returned. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 0;

    while (count < n) {
        ++m;                      /* examine the next integer */

        /* compute the Luhn check digit for m */
        idx_t sum = 0;
        bool odd = true;          /* position 1 from the right is odd */
        idx_t x = m;

        while (x > 0) {
            idx_t digit = x % 10;
            if (odd) {            /* double odd positions */
                digit *= 2;
                if (digit > 9) digit -= 9;
            }
            sum += digit;
            x /= 10;
            odd = !odd;
        }

        idx_t d = (10 - (sum % 10)) % 10;
        if (d == 1)
            ++count;              /* found a term */
    }

    return (seq_t)m;
}
