/*
 * A214162  Number of 0..5 colorings of an nX5 array circular in the 5
 *          direction with new values 0..5 introduced in row major order.
 * https://oeis.org/A214162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1402*a(n-1) - 130492*a(n-2) + 2979072*a(n-3) * -
 *            15573492*a(n-4) + 12571416*a(n-5) * for n > 5. * Initial values:
 *            * a(1) = 11 * a(2) = 5994 * a(3) = 7386816 * a(4) = 9601970064 *
 *            a(5) = 12515731371696
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;

    /* Initial values for n = 1 .. 5 */
    if (n == 1) return (seq_t)11;
    if (n == 2) return (seq_t)5994;
    if (n == 3) return (seq_t)7386816;
    if (n == 4) return (seq_t)9601970064;
    if (n == 5) return (seq_t)12515731371696;

    /* Use a sliding window of the last five terms */
    seq_t a1 = (seq_t)11;               /* a(n-5)   */
    seq_t a2 = (seq_t)5994;             /* a(n-4)   */
    seq_t a3 = (seq_t)7386816;          /* a(n-3)   */
    seq_t a4 = (seq_t)9601970064;       /* a(n-2)   */
    seq_t a5 = (seq_t)12515731371696;   /* a(n-1)   */

    for (idx_t i = 6; i <= n; ++i) {
        /* Compute the next term using the recurrence.
           All intermediate results stay non‑negative, so
           unsigned arithmetic is safe. */
        seq_t term1 = 1402 * a5 + 2979072 * a3 + 12571416 * a1;
        seq_t term2 = 130492 * a4 + 15573492 * a2;
        seq_t an = term1 - term2;

        /* Shift the window for the next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }
    return a5;
}
