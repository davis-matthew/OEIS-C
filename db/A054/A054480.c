/*
 * A054480  Number of different positive braids with n crossings of 4 strands.
 * https://oeis.org/A054480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 2*a(n-3) + a(n-6), for n ≥ 6, with
 *            a(0)=1, a(1)=3, a(2)=8, a(3)=19, a(4)=43, a(5)=94
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Allocate a static buffer large enough for expected n (≤ 10000). */
    enum { MAX_N = 20001 };
    static seq_t seq[MAX_N];
    static idx_t max_calculated = 0;
    static bool initialized = false;

    if (!initialized) {
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)3;
        seq[2] = (seq_t)8;
        seq[3] = (seq_t)19;
        seq[4] = (seq_t)43;
        seq[5] = (seq_t)94;
        max_calculated = 5;
        initialized = true;
    }

    if (n <= max_calculated) {
        return seq[n];
    }

    for (idx_t i = max_calculated + 1; i <= n; ++i) {
        seq[i] = seq[i-1] * (seq_t)3;
        seq[i] -= seq[i-2];
        seq[i] -= seq[i-3] * (seq_t)2;
        seq[i] += seq[i-6];
    }
    max_calculated = n;
    return seq[n];
}
