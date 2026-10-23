/*
 * A040537  Continued fraction for sqrt(561).
 * https://oeis.org/A040537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(561).
   a(0) = floor(sqrt(561)) = 23.
   For n ≥ 1 the sequence repeats the 14 terms
   1, 2, 5, 1, 1, 2, 2, 2, 1, 1, 5, 2, 1, 46.
   The function computes this periodic sequence. */

#include <stdbool.h>
#include <stdint.h>

/* The function producing the n‑th term of A040537. */
seq_t a(idx_t n)
{
    /* static state – computed once on first call */
    static bool initialized = false;
    static seq_t period[32];
    static idx_t period_len = 0;

    if (!initialized) {
        /* constants for √561 */
        const seq_t N   = 561;  /* radicand   */
        const seq_t A0  = 23;   /* floor√N    */

        /* classical continued‑fraction expansion of √N */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a = A0;

        /* generate the periodic part until a == 2·A0 (the last term) */
        do {
            m = d * a - m;
            d = (N - m * m) / d;
            a = (A0 + m) / d;
            period[period_len++] = a;
        } while (a != 2 * A0);

        initialized = true;
    }

    if (n == 0) {
        return 23;            /* floor√561 */
    }

    /* For n ≥ 1, return the (n‑1)th element of the period */
    idx_t idx = (n - 1) % period_len;
    return period[idx];
}
