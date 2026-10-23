/*
 * A011690  A binary m-sequence: expansion of reciprocal of x^7+x^3+1.
 * https://oeis.org/A011690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑3) XOR a(n‑7); a(0)=0, a(1)=0, a(2)=0, a(3)=0,
 *            a(4)=0, a(5)=0, a(6)=1. The sequence is 127‑periodic
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t period[127];
static int period_ready = 0;

static void init_period(void)
{
    if (period_ready) return;

    /* initial seed: six zeros then one */
    period[0] = 0;
    period[1] = 0;
    period[2] = 0;
    period[3] = 0;
    period[4] = 0;
    period[5] = 0;
    period[6] = 1;

    /* recurrence a(n) = a(n‑3) XOR a(n‑7) */
    for (idx_t i = 7; i < 127; ++i) {
        seq_t x = period[i - 3];
        x ^= period[i - 7];
        period[i] = x;
    }

    period_ready = 1;
}

seq_t a(idx_t n)
{
    init_period();
    /* 127‑periodicity */
    idx_t m = n % 127;
    return period[m];
}
