/*
 * A118862  Start with 1 and repeatedly place the first digit at the end of
 *          the number and add 15.
 * https://oeis.org/A118862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1, a(2) = 16.
 * For n ≥ 3, a(n) is obtained from a(n‑1) by taking the first decimal
 * digit, moving it to the end, and adding 15:
 *   let d be the first digit of a(n‑1), k the number of remaining
 *   digits, and r the rest of the number:
 *     a(n‑1) = d·10ᵏ + r
 *   then
 *     a(n)   = r·10 + d + 15
 *   For n > 8 the sequence is periodic with period 6, so a(n) = a(n‑6). */

#include <stdint.h>

static seq_t next_val(seq_t x)
{
    seq_t temp = x;
    seq_t pow10 = 1;
    while (temp >= 10)          /* find first digit and 10ᵏ */
    {
        temp   /= 10;
        pow10  *= 10;
    }
    seq_t first = temp;         /* first decimal digit */
    seq_t rest  = x - first * pow10;  /* remaining digits */

    return rest * 10 + first + 15;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 16;

    /* Reduce n to the range 3…8 using the 6‑term cycle */
    if (n >= 9)
        n = 3 + ((n - 3) % 6);

    seq_t val = 1;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
        val = next_val(val);

    return val;
}
