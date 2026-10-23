/*
 * A040501  Continued fraction for sqrt(524).
 * https://oeis.org/A040501
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

/* Calculates the continued‑fraction terms of √524.
   a(0)=⌊√524⌋=22; for n≥1 the terms repeat with period
   1,8,5,1,1,1,1,2,1,10,1,2,1,1,1,1,5,8,1,44, …          */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool init_done = false;
    static seq_t period[64];
    static size_t period_len = 0;
    static const seq_t a0 = 22;             /* floor(sqrt(524)) */

    if (!init_done) {
        /* Compute the period of the continued fraction of √524. */
        const seq_t N = 524;
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_ = a0;
        size_t i = 0;

        do {
            m  = d * a_ - m;
            d  = (N - m * m) / d;
            a_ = (a0 + m) / d;
            period[i++] = a_;
        } while (a_ != 2 * a0);          /* period ends when a_ == 2*a0 */

        period_len = i;
        init_done  = true;
    }

    if (n == 0) {
        return a0;
    }

    size_t idx = (size_t)((n - 1) % period_len);   /* periodic part starts at a(1) */
    return period[idx];
}
