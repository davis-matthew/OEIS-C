/*
 * A040581  Continued fraction for sqrt(606).
 * https://oeis.org/A040581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   continued fraction terms of sqrt(606)
 * Range:     u8 none, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Computes the nth term of the continued fraction of sqrt(606).  This
   sequence is periodic with period length 20 after the initial term
   a(0)=24.  It is generated on first call and cached for subsequent calls. */
seq_t a(idx_t n)
{
    /* a0 = floor(sqrt(606)) */
    static bool ready = false;
    static seq_t a0;
    static seq_t period[20];
    static size_t period_len = 0;

    if (!ready) {
        /* Compute a0 by brute‑force integer square root */
        seq_t D = 606;
        seq_t i = 0;
        while ((i+1)*(i+1) <= D)
            i += 1;
        a0 = i;

        /* Compute continued fraction terms until the period terminates
           at a value of 2*a0.  For sqrt(606) this happens after
           20 terms. */
        seq_t m = 0;
        seq_t d = 1;
        seq_t curr = a0;
        while (1) {
            m = d * curr - m;
            d = (D - m * m) / d;
            curr = (a0 + m) / d;
            period[period_len++] = curr;
            if (curr == 2 * a0)    /* period finished */
                break;
        }
        ready = true;
    }

    if (n == 0)
        return a0;

    /* zero‑based index into period array: a(1) is period[0] */
    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
