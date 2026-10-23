/*
 * A102460  a(n) = 1 if n is a Lucas number, else a(n) = 0.
 * https://oeis.org/A102460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a Lucas number, else 0
 * Range:     u8 n <= 9694, u16 n <= 9694, u32 n <= 9694, u64 n <= 9694,
 *            u128 n <= 9694, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return 1 iff the non-negative integer n is a Lucas number. */
seq_t a(idx_t n)
{
    /* Lucas numbers: L0 = 2, L1 = 1, Ln = Ln-1 + Ln-2. */
    if (n == 0)                /* 0 is not a Lucas number */
        return 0;

    if (n == 1 || n == 2)      /* 1 and 2 are Lucas numbers */
        return 1;

    seq_t target = (seq_t)n;

    seq_t l_prev = 2;          /* L0 */
    seq_t l_curr = 1;          /* L1 */

    /* Check the first two Lucas numbers explicitly. */
    if (l_prev == target) return 1;
    if (l_curr == target) return 1;

    while (true) {
        seq_t l_next = l_prev + l_curr;

        if (l_next == target) return 1;   /* Found the Lucas number */
        if (l_next > target)  break;      /* Sequence has exceeded the target */

        l_prev = l_curr;                  /* shift for next iteration */
        l_curr = l_next;
    }
    return 0;
}
