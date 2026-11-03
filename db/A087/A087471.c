/*
 * A087471  Final digit resulting from iterations of the product of the two
 *          numbers formed from the alternating digits of n.
 * https://oeis.org/A087471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(f(n)), where f(n) is Murthy's function: * f(n) =
 *            (number formed by digits at odd positions) * * (number formed by
 *            digits at even positions) * The sequence a(n) is the terminal
 *            single digit obtained by * iterating f until a one‑digit number
 *            remains
 * Range:     u8 n <= 303, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 121 terms through n = 976 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: apply Murthy's function to a seq_t value >= 10.
 * It returns the product of the two numbers formed from the
 * alternating digits of the input. */
static seq_t murthy_func(seq_t x)
{
    /* Collect digits in reverse order (least significant first). */
    int digits[32];          /* enough for 64‑bit numbers */
    int len = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        digits[len++] = (int)(tmp % 10);
        tmp = tmp / 10;
    }
    if (len == 0)            /* x == 0 */
        return 0;

    /* Build the two numbers from left to right. */
    seq_t num_odd  = 0;
    seq_t num_even = 0;
    int pos = 0;             /* position from the most significant digit (0‑based) */
    for (int i = len - 1; i >= 0; --i, ++pos) {
        int d = digits[i];
        if ((pos & 1) == 0)        /* odd position (1,3,5,…) */
            num_odd = num_odd * 10 + d;
        else                       /* even position (2,4,6,…) */
            num_even = num_even * 10 + d;
    }
    return num_odd * num_even;
}

/* Main sequence function.  Input index n (offset 1). */
seq_t a(idx_t n)
{
    seq_t v = (seq_t)n;
    while (v >= 10)                /* repeat until single digit */
        v = murthy_func(v);
    return v;
}
