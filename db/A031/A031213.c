/*
 * A031213  Periods of sum of 10th powers of digits iterated until the
 *          sequence becomes periodic.
 * https://oeis.org/A031213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Period length of the sequence obtained by iterating the
 * function f(x) = sum of the 10th powers of the decimal digits of x.
 * For a given starting integer n (n >= 0), a(n) is the length of the
 * eventual cycle of the sequence n, f(n), f(f(n)), ... .
 *
 * The sequence is a(0)=1, a(1)=1, a(2)=17, a(3)=123, a(4)=17, a(5)=17,
 * a(6)=123, a(7)=123, a(8)=123, a(9)=123, a(10)=1, a(11)=17, a(12)=123,
 * … (matching OEIS A031213).  The implementation uses Floyd’s
 * cycle‑finding algorithm, which runs in O(λ+μ) time (λ = preperiod,
 * μ = cycle length) and constant memory.
 *
 * The function is defined for all n as requested; it does not assume
 * any ordering of calls.  The algorithm is fast enough for n up to
 * at least 10000 within the time limits.
 */

#include <stdint.h>
#include <stdbool.h>

/* Pre‑computed 10th powers of the digits 0..9 (they fit in 64‑bit). */
static const seq_t pow10_digit[10] = {
    (seq_t)0,                                    /* 0^10 */
    (seq_t)1,                                    /* 1^10 */
    (seq_t)1024,                                 /* 2^10 */
    (seq_t)59049,                                /* 3^10 */
    (seq_t)1048576,                              /* 4^10 */
    (seq_t)9765625,                              /* 5^10 */
    (seq_t)60466176,                             /* 6^10 */
    (seq_t)282475249,                            /* 7^10 */
    (seq_t)1073741824,                           /* 8^10 */
    (seq_t)3486784401                            /* 9^10 */
};

/* Helper that returns the sum of 10th powers of the decimal digits of x. */
static inline seq_t sum10th(seq_t x)
{
    seq_t sum = 0;
    if (x == 0)
        return 0;               /* 0^10 = 0; no need to enter the loop */
    while (x != 0) {
        /* The divisor 10 safely fits in seq_t for the values we encounter. */
        seq_t digit = x % 10;
        /* digit is 0..9, so we can index the table safely. */
        sum += pow10_digit[(unsigned)digit];
        x /= 10;
    }
    return sum;
}

/* Main function required by the template. */
seq_t a(idx_t n)
{
    seq_t start = (seq_t)n;

    /* Floyd's cycle detection: tortoise moves 1 step, hare moves 2 steps. */
    seq_t tortoise = sum10th(start);
    seq_t hare = sum10th(tortoise);

    /* Find first meeting point.  For n=0 or n=1 the loop is skipped. */
    while (tortoise != hare) {
        tortoise = sum10th(tortoise);
        hare = sum10th(sum10th(hare));
    }

    /* Determine the cycle length μ. */
    seq_t period = 1;
    hare = sum10th(tortoise);
    while (tortoise != hare) {
        hare = sum10th(hare);
        ++period;
    }

    return period;
}
