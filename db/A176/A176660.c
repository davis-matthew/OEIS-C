/*
 * A176660  Partial sums of A038772.
 * https://oeis.org/A176660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = sum of the first n integers that are *not* divisible by
 *            any of their (non‑zero) decimal digits The OEIS A176660 is
 *            defined as a(n) = Σ_{k=1..n} A038772(k) where A038772 gives the
 *            successive numbers that are not divisible by any of their
 *            digits. This function computes that partial sum by br...
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t sum   = 0;   /* running total of qualifying numbers */
    idx_t count = 0;   /* how many qualifying numbers we have found */
    seq_t num   = 0;   /* candidate number */

    while (count < n) {
        num++;  /* consider the next integer */

        /* Quick reject any number that contains digit 1 – it is
           always divisible by 1. */
        seq_t temp = num;
        bool has_one = false;
        while (temp > 0) {
            int d = (int)(temp % 10);
            if (d == 1) { has_one = true; break; }
            temp /= 10;
        }
        if (has_one) continue;

        /* Now check remaining digits.  Zero digits are ignored
           because division by zero is undefined. */
        temp = num;
        bool ok = true;
        while (temp > 0) {
            int d = (int)(temp % 10);
            if (d == 0) { temp /= 10; continue; }
            if ((num % d) == 0) { ok = false; break; }
            temp /= 10;
        }

        if (ok) {
            sum += num;
            count++;
        }
    }

    return sum;
}
