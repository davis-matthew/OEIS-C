/*
 * A154060  Number of planar triangular n X n X n nonnegative integer grids
 *          symmetric under 120 degree rotation with every similarly oriented
 *          5 X 5 X 5 subtriangle summing to 6.
 * https://oeis.org/A154060
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 20 for n ≥ 17 and n ≡ 1 (mod 5) * a(n) = 48 for n ≥ 17
 *            and n ≠ 1 (mod 5) * a(n) = 15,48,136,132,… for 5 ≤ n ≤ 16 (see
 *            table below) * a(n) is 0 for n < 5 (unused)
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 5 ≤ n ≤ 16 – pre‑computed values from the OEIS definition */
    if (n < 5)
        return (seq_t)0;                /* undefined for n < 5, harmless */

    if (n < 17) {
        static const seq_t vals[17] = {
            0, 0, 0, 0, 0,     /* 0-4 – unused */
            15,                /* n = 5  */
            48,                /* n = 6  */
            136,               /* n = 7  */
            132,               /* n = 8  */
            79,                /* n = 9  */
            180,               /* n =10  */
            173,               /* n =11  */
            60,                /* n =12  */
            137,               /* n =13  */
            52,                /* n =14  */
            50,                /* n =15  */
            22                 /* n =16  */
        };
        return vals[(size_t)n];
    }

    /* n ≥ 17 – simple periodic pattern 48,48,48,48,20 (length 5) */
    if ((n % 5ULL) == 1ULL)
        return (seq_t)20;
    else
        return (seq_t)48;
}
