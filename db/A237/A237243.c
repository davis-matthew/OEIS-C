/*
 * A237243  Number of (n+1) X (2+1) 0..2 arrays with the maximum plus the
 *          upper median minus the lower median minus the minimum of every 2 X
 *          2 subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A237243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1410*a(n-4) for n>6, with a(1)=344, a(2)=2064,
 *            a(3)=12600, a(4)=78420, a(5)=477144, a(6)=2908188
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial values for n = 1..6 */
    static const seq_t init[7] = {
        (seq_t)0,              /* unused index 0 */
        (seq_t)344,
        (seq_t)2064,
        (seq_t)12600,
        (seq_t)78420,
        (seq_t)477144,
        (seq_t)2908188
    };

    if (n <= 6)
        return init[n];

    /* Determine the base index from the 4–cycle */
    unsigned int r = (unsigned int)(n % 4u);
    idx_t base;
    switch (r) {
        case 3: base = 3; break;   /* n % 4 == 3 => base = 3   */
        case 0: base = 4; break;   /* n % 4 == 0 => base = 4   */
        case 1: base = 5; break;   /* n % 4 == 1 => base = 5   */
        case 2: base = 6; break;   /* n % 4 == 2 => base = 6   */
        default: base = 6; break;   /* safety fallback */
    }

    /* Compute exponent e = (n - base) / 4 */
    idx_t e = (n - base) / 4u;

    seq_t result = init[base];
    seq_t mult   = (seq_t)1410;

    /* multiply by 1410^e */
    for (idx_t i = 0; i < e; ++i)
        result *= mult;

    return result;
}
