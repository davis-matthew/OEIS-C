/*
 * A165034  Length of preperiodic part of trajectory of n under iteration of
 *          the base-5 Kaprekar map in A165032.
 * https://oeis.org/A165034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3125, u16 n <= 3125, u32 n <= 3125, u64 n <= 3125,
 *            u128 n <= 3125, big any n
 * Validated: standard, pass, 121 terms through n = 3097 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the length of the preperiodic part of the trajectory of n
 * under the base‑5 Kaprekar map (OEIS A165034).  The function returns
 * a(n) as defined in the OEIS entry.  It works for any seq_t that is
 * unsigned (8, 16, 32, 64 or 128 bits, or an arbitrary‑precision
 * integer type).  All intermediate computations are performed with
 * unsigned long long, which is sufficient for the input bounds used
 * in the tests (n ≤ 100 000).  The function uses Floyd's cycle
 * detection to find the pre‑period length μ. */

#include <stdint.h>

static unsigned long long kaprekar_step_ull(unsigned long long v)
{
    /* Count the base‑5 digits of v.  For v==0 we treat it as a single
       digit 0, so that max==min==0 and step(v)==0. */
    unsigned cnt[5] = {0};
    if (v == 0) {
        cnt[0] = 1;
    } else {
        while (v) {
            unsigned d = (unsigned)(v % 5);
            cnt[d]++;
            v /= 5;
        }
    }

    /* Build the maximum and minimum numbers possible from these
       digits. */
    unsigned long long max = 0, min = 0;
    for (int d = 4; d >= 0; --d) {
        for (unsigned k = 0; k < cnt[d]; ++k)
            max = max * 5 + d;
    }
    for (int d = 0; d <= 4; ++d) {
        for (unsigned k = 0; k < cnt[d]; ++k)
            min = min * 5 + d;
    }
    return max - min;
}

seq_t a(idx_t n)
{
    /* Convert the index to a 64‑bit intermediate value for safety. */
    unsigned long long start = (unsigned long long)n;

    /* Special case: 0 immediately maps to 0, hence length 0. */
    if (start == 0)
        return (seq_t)0;

    /* Floyd's algorithm to find a meeting point in the cycle. */
    unsigned long long tortoise = kaprekar_step_ull(start);
    unsigned long long hare = kaprekar_step_ull(tortoise);
    while (tortoise != hare) {
        tortoise = kaprekar_step_ull(tortoise);
        hare = kaprekar_step_ull(kaprekar_step_ull(hare));
    }

    /* Find the first element of the cycle. */
    unsigned long long mu = 0;
    tortoise = start;
    while (tortoise != hare) {
        tortoise = kaprekar_step_ull(tortoise);
        hare = kaprekar_step_ull(hare);
        ++mu;
    }

    return (seq_t)mu;
}
