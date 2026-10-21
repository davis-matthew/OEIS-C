/*
 * A063992  Numbers that are not factorials.
 * https://oeis.org/A063992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer that is not equal to k! for any
 *            k ≥ 0
 * Range:     u8 n <= 249, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Offset starts at 1: a(1) = 3 */
    if (n == 0) return 0;

    seq_t i = 1;           /* current candidate integer */
    seq_t fact = 1;        /* current factorial value (k!) */
    idx_t k = 1;           /* index of the current factorial */
    seq_t next_fact = fact;/* value of the factorial to be skipped */
    idx_t count = 0;       /* number of non‑factorials encountered */

    while (1) {
        if (i == next_fact) {
            /* i is a factorial – skip it and advance to the next factorial */
            ++k;
            fact = mul_small(fact, (unsigned int)k);
            next_fact = fact;
        } else {
            /* i is not a factorial */
            ++count;
            if (count == n) {
                return i;
            }
        }
        ++i;
    }
}
