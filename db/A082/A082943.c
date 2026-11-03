/*
 * A082943  Positive numbers not divisible by any of their digits nor by the
 *          sum of their digits.
 * https://oeis.org/A082943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 23, * a(n) = the n-th positive integer that has no digit
 *            zero and is not * divisible by any of its decimal digits nor by
 *            the sum of its digits
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 950 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A straightforward search.  For each integer starting from 1 we
 *  test whether all of its digits are non‑zero and whether the
 *  number is not divisible by any of those digits or by its digit sum.
 *  The search stops once the n‑th such number has been found.  */

#include <stdbool.h>

/*  Return true iff v satisfies the conditions of A082943.  */
static bool a082943_is_valid(seq_t v)
{
    seq_t tmp = v;
    seq_t digit_sum = 0;

    while (tmp > 0) {
        seq_t d = tmp % 10;          /* current decimal digit          */
        if (d == 0)                  /* digit zero is forbidden        */
            return false;
        if (v % d == 0)              /* v divisible by this digit?     */
            return false;
        digit_sum += d;              /* accumulate digit sum           */
        tmp /= 10;
    }

    if (digit_sum == 0)              /* should not occur (v>0)          */
        return false;
    if (v % digit_sum == 0)          /* divisible by sum of digits?    */
        return false;

    return true;
}

/*  External entry point required by the build.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* not used, but harmless          */

    idx_t found = 0;
    seq_t val = 1;

    while (1) {
        if (a082943_is_valid(val)) {
            found++;
            if (found == n)
                return val;
        }
        val++;                       /* next candidate                  */
    }
}
