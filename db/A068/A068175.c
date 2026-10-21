/*
 * A068175  a(1) = 1; for n>1, a(n) is the smallest square > a(n-1) obtained
 *          by inserting digits into a(n-1).
 * https://oeis.org/A068175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1
   a(2) = 16
   a(3) = 169
   a(4) = 1369
   a(5) = 13689
   For n ≥ 6, a(n) = (3 followed by (n‑5) digits '6' then 7)² */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the first five terms explicitly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)16;
    if (n == 3) return (seq_t)169;
    if (n == 4) return (seq_t)1369;
    if (n == 5) return (seq_t)13689;

    /* for n ≥ 6: build the root “3” + (n‑5)×“6” + “7” */
    idx_t sixes = n - 5;          /* number of 6's */
    seq_t root = (seq_t)3;        /* start with the leading 3 */

    for (idx_t i = 0; i < sixes; ++i)
        root = root * (seq_t)10 + (seq_t)6;   /* append a 6 */

    root = root * (seq_t)10 + (seq_t)7;       /* append the final 7 */

    return root * root;     /* square the root */
}
