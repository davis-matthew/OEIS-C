/*
 * A177849  The number of ways of minimal weight to make change for n cents
 *          using fairly valued United States coins (copper 1-cent coin, a
 *          nickel 5-cent coin, and silver 10-cent and 25-cent coins) assuming
 *          that silver is more valuable than nickel and that nickel is more
 *          valuable than copper.
 * https://oeis.org/A177849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the number of non‑negative integer solutions * (a,b) to
 *            2*a + 5*b = k, where k is the largest integer ≤ ⌊n/5⌋ * that is
 *            not equal to 1 or 3 (the only values of ⌊n/5⌋ that are not *
 *            representable by 10‑ and 25‑cent coins). * This counts the ways
 *            to make minimal‑weight change for n cents *...
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Step 1: find the greatest representable k ≤ ⌊n/5⌋ */
    idx_t M = n / 5;          /* floor(n/5)  */
    idx_t k = M;
    if (M == 1 || M == 3)    /* 1 and 3 are not representable by 2a+5b */
        k = M - 1;           /* choose the next smaller representable value */

    /* Step 2: count solutions to 2a + 5b = k */
    idx_t m = k / 5;          /* maximum possible b (i.e., many 25‑cent coins) */
    idx_t parity = k & 1;     /* parity constraint: b ≡ k (mod 2) */
    idx_t count;
    if (m >= parity) {
        count = ((m - parity) >> 1) + 1;   /* number of valid b values */
    } else {
        count = 0;
    }

    return (seq_t)count;
}
