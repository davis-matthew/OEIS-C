/*
 * A090327  Number of rules of a context-free grammar in Chomsky normal form
 *          that generates all permutations of n symbols.
 * https://oeis.org/A090327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=4, a(3)=11, a(4)=30,
 *            a(n)=6*a(n-1)-11*a(n-2)+6*a(n-3) for n>=5
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Handle the small indices directly. */
    if (n == 1) return 1;
    if (n == 2) return 4;
    if (n == 3) return 11;
    if (n == 4) return 30;

    /* Compute iteratively to avoid any dynamic allocation that
       might be problematic when seq_t is an arbitrary‑precision class. */
    seq_t a1 = 1;   /* a(1)   */
    seq_t a2 = 4;   /* a(2)   */
    seq_t a3 = 11;  /* a(3)   */
    seq_t a4 = 30;  /* a(4)   */
    seq_t ai;       /* current term */

    for (idx_t i = 5; i <= n; ++i) {
        ai = 6 * a4 - 11 * a3 + 6 * a2;   /* recurrence */
        a2 = a3;   /* shift the window */
        a3 = a4;
        a4 = ai;
    }

    return a4;
}
