/*
 * A206213  Number of (n+1)X7 0..2 arrays with every 2X3 or 3X2 subblock
 *          having exactly two equal edges, and new values 0..2 introduced in
 *          row major order.
 * https://oeis.org/A206213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 130 * a(n-3) for n > 9 initial values: a(1) = 8802 a(2) =
 *            9072 a(3) = 14416 a(4) = 74080 a(5) = 359424 a(6) = 1764000 a(7)
 *            = 9438912 a(8) = 46540800 a(9) = 228988224 These are the only
 *            values needed beyond which the recurrence applies
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* base values for 1 ≤ n ≤ 9 */
    static const seq_t base[9] = {
        8802ULL,      /* a(1) */
        9072ULL,      /* a(2) */
        14416ULL,     /* a(3) */
        74080ULL,     /* a(4) */
        359424ULL,    /* a(5) */
        1764000ULL,   /* a(6) */
        9438912ULL,   /* a(7) */
        46540800ULL,  /* a(8) */
        228988224ULL  /* a(9) */
    };

    if (n <= 9)
        return base[n - 1];

    /* Determine the appropriate starting index (7, 8, or 9) based on n mod 3 */
    idx_t base_index;
    if (n % 3 == 1)
        base_index = 7; /* use a(7) */
    else if (n % 3 == 2)
        base_index = 8; /* use a(8) */
    else
        base_index = 9; /* use a(9) */

    seq_t result = base[base_index - 1];
    idx_t steps  = (n - base_index) / 3;            /* number of 130‑multiplies */

    /* multiply by 130^steps */
    for (idx_t i = 0; i < steps; ++i)
        result *= (seq_t)130;

    return result;
}
