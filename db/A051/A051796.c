/*
 * A051796  a(n)=sum((-1)^i*a(i),i=n-9..n-1); with a(i)=i for i=1..9.
 * https://oeis.org/A051796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = sum_{i=n-9}^{n-1} (-1)^i * a(i), with a(i)=i for 1≤i≤9,
 *            offset 1, periodicity: a(n+20)=a(n)
 * Range:     i8 n <= 101, i16 n <= 101, i32 n <= 101, i64 n <= 101,
 *            i128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t period[20];
static bool init_done = false;

static void init_period(void) {
    if (init_done) return;
    /* Base values a(1)..a(9) */
    for (uint64_t i = 1; i <= 9; ++i)
        period[i-1] = (seq_t)i;

    /* Recurrence for a(10)..a(20) */
    for (uint64_t n = 10; n <= 20; ++n) {
        seq_t sum = 0;
        for (uint64_t k = n - 9; k <= n - 1; ++k) {
            seq_t term = period[k-1];
            if ((k & 1ULL) == 0) {   /* (-1)^k == 1 when k even */
                sum += term;
            } else {                 /* (-1)^k == -1 when k odd */
                sum -= term;
            }
        }
        period[n-1] = sum;
    }
    init_done = true;
}

seq_t a(idx_t n) {
    init_period();
    /* OEIS offset 1, use 1‑based modulo 20 */
    idx_t idx = (n - 1) % 20;
    return period[idx];
}
