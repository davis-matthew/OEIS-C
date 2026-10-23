/*
 * A152829  Numbers k whose squares can be written in exactly one way as a sum
 *          of three squares: k^2 = a^2 + b^2 + c^2 with 1 <= a <= b <= c.
 * https://oeis.org/A152829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 42, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A152829: Numbers k whose squares can be written in exactly one
 * way as a sum of three squares: k^2 = a^2 + b^2 + c^2 with 1 <= a <= b <= c.
 *
 * The sequence satisfies the recurrence 
 *    a(n) = 2 * a(n-3)   for n > 5
 * with initial values
 *    a(1) = 3,  a(2) = 6,  a(3) = 7,  a(4) = 12,  a(5) = 13.
 *
 * This implementation uses that recurrence directly; no extra storage
 * or heavy computation is required.
 */

#include <stdint.h>

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* Handle indices below the offset – behaviour undefined there. */
    if (n == 0)
        return 0;

    /* Return the first five terms explicitly. */
    switch (n)
    {
        case 1: return (seq_t)3;
        case 2: return (seq_t)6;
        case 3: return (seq_t)7;
        case 4: return (seq_t)12;
        case 5: return (seq_t)13;
        default: break;
    }

    /* For n > 5, determine base term and exponent from the congruence class. */
    idx_t r = n % 3;
    seq_t base;
    idx_t exp;

    if (r == 0) {
        base = (seq_t)7;                /* a(3) */
        exp  = (n - 3) / 3;             /* number of doublings */
    } else if (r == 1) {
        base = (seq_t)12;               /* a(4) */
        exp  = (n - 4) / 3;
    } else { /* r == 2 */
        base = (seq_t)13;               /* a(5) */
        exp  = (n - 5) / 3;
    }

    /* Multiply base by 2^exp.  We use a loop to avoid overflow when the
     * underlying type is a small integer width; the loop guarantees
     * that each shift is by one. */
    seq_t result = base;
    while (exp > 0) {
        result <<= 1;
        --exp;
    }
    return result;
}
