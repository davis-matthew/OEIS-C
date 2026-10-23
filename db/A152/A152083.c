/*
 * A152083  Number of occurrences of "I" in all Roman numerals of numbers from
 *          1 to n.
 * https://oeis.org/A152083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, nonn, word
 * Formula:   a(n) = a(n-5) + 7 for n > 5; a(1) = 1, a(2) = 3, a(3) = 6, a(4)
 *            = 7, a(5) = 7
 * Range:     u8 n <= 182, u16 n <= 3999, u32 n <= 3999, u64 n <= 3999,
 *            u128 n <= 3999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base counts for the first five terms.  */
    static const seq_t base[6] = {0, 1, 3, 6, 7, 7};  /* base[0] unused */

    if (n <= 5)
        return base[(size_t)n];

    /* For n > 5, write n = 5*k + r , 1 <= r <= 5.
       Then a(n) = base[r] + 7*k.  */
    idx_t k = (n - 1) / 5;                /* number of full 5‑step blocks   */
    size_t r = ((n - 1) % 5) + 1;          /* remainder in 1..5              */

    return base[r] + (seq_t)k * 7;
}
