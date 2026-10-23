/*
 * A168950  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A168950
 *
 * Formula: a(0) = 1,  a(1) = 33,
 *          a(n) = 33 * 32^(n-1) for 2 ≤ n ≤ 21
 *          For n ≥ 22:
 *          a(n) = 31 * (a(n-1)+a(n-2)+…+a(n-21)) - 496 * a(n-22)
 *
 * The recurrence follows from the rational generating function
 * provided in the OEIS entry.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)33;

    /* returns value for 2 ≤ n ≤ 21 */
    if (n < 22) {
        seq_t res = (seq_t)33;
        for (idx_t i = 2; i <= n; ++i) {
            res = res * (seq_t)32;
        }
        return res;
    }

    /* initial 22 terms a[0] … a[21] */
    const size_t INIT = 22;
    seq_t buf[INIT];
    buf[0] = (seq_t)1;
    for (idx_t i = 1; i < INIT; ++i) {
        buf[i] = buf[i - 1] * (seq_t)32;
    }

    /* sum of all 22 values in the buffer */
    seq_t sum_all = buf[0];
    for (size_t i = 1; i < INIT; ++i) {
        sum_all += buf[i];
    }

    size_t head = 0;          /* index of a(n-22) */
    seq_t next = 0;           /* will hold a(n) */

    for (idx_t cur = 22; cur <= n; ++cur) {
        /* sum of the 21 most recent values */
        seq_t sumPrev21 = sum_all - buf[head];

        /* a(cur) = 31 * sumPrev21 - 496 * buf[head] */
        seq_t term1 = (seq_t)31 * sumPrev21;
        seq_t term2 = (seq_t)496 * buf[head];
        next = term1 - term2;

        /* update the buffer for the next iteration */
        sum_all = sum_all - buf[head] + next;
        head = (head + 1) % INIT;
        buf[head] = next;
    }

    return next;
}
