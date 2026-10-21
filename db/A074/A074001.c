/*
 * A074001  Number of elements of GF(3^n) with trace 0 and subtrace 1.
 * https://oeis.org/A074001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1) = 0 a(2) = 2 a(3) = 0 For n ≥ 4: a(n) = 3^(n-2) +
 *            3^((n-2)/2) if n is even and n mod 12 ∈ {0,2,4} a(n) = 3^(n-2) –
 *            3^((n-2)/2) if n is even and n mod 12 ∈ {6,8,10} a(n) = 3^(n-2)
 *            + 3^e if n is odd and ⌊(n-5)/6⌋ is even a(n) = 3^(n-2) – 3^e if
 *            n is odd and ⌊(n-5)/6⌋ is odd where m = (n-1)/2,...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly */
    if (n == 1) return 0;
    if (n == 2) return 2;
    if (n == 3) return 0;

    /* Base term 3^(n-2) */
    seq_t base = pow3(n - 2);

    if ((n & 1) == 0) { /* even n */
        seq_t mid = pow3((n - 2) / 2);
        idx_t r = n % 12;
        /* r in {6,8,10} => subtract, else add */
        if (r == 6 || r == 8 || r == 10)
            return base - mid;
        else
            return base + mid;
    } else {            /* odd n */
        idx_t m = (n - 1) / 2;
        idx_t e = (m % 3 == 1) ? m : (m - 1);
        seq_t diff = pow3(e);
        idx_t k = (n - 5) / 6;   /* floor */
        if ((k & 1) == 0)         /* even k -> add */
            return base + diff;
        else                      /* odd k  -> subtract */
            return base - diff;
    }
}
