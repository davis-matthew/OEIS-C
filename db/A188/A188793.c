/*
 * A188793  Start with 1 and 5, then repeatedly adjoin the smallest number
 *          that is greater than the last term and not equal to the sum of a
 *          subset of the existing terms.
 * https://oeis.org/A188793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=5, a(3)=7; for h≥2 a(2h) = 10·3^(h–2) – 1 a(2h+1) =
 *            10·3^(h–2) + 1 (odd n)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)7;

    /* For n≥4 use the closed form */
    idx_t h;
    bool even = (n & 1ULL) == 0;            /* n even? */
    if (even)
        h = n / 2;      /* n = 2h  → h = n/2  */
    else
        h = (n - 1) / 2;/* n = 2h+1 → h = (n-1)/2 */

    /* Compute 3^(h-2) */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < h - 2; ++i)
        pow3 *= (seq_t)3;

    /* 10 * 3^(h-2) ± 1 */
    seq_t result = (seq_t)10 * pow3;
    if (even)
        result -= (seq_t)1;
    else
        result += (seq_t)1;

    return result;
}
