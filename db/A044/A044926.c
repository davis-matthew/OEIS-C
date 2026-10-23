/*
 * A044926  a(n)=so-se, where so(se)=sum of odd(even) base 4 run lengths of n.
 * https://oeis.org/A044926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A044926 – sum of odd‑run lengths minus sum of even‑run lengths
 *  of the base‑4 representation of n.
 *
 *  For each decimal n ≥ 1:
 *      1. Write n in base‑4.
 *      2. Partition the representation into runs of equal digits.
 *      3. Let so be the sum of the lengths of runs whose length is odd.
 *      4. Let se be the sum of the lengths of runs whose length is even.
 *      5. a(n) = so – se.
 *
 *  The routine works in O(log₄ n) time and uses only a few bytes of
 *  temporary storage. */

#include <stdint.h>   /* for idx_t, seq_t and uint64_t */

seq_t a(idx_t n)
{
    if (n == 0)          /* no term defined for n = 0; return 0 safely */
        return 0;

    /* base‑4 digits – at most 32 for a 64‑bit integer */
    int digits[32];
    int len = 0;
    idx_t temp = n;

    /* extract digits least‑significant first */
    while (temp > 0) {
        digits[len++] = (int)(temp & 3);   /* temp % 4 */
        temp >>= 2;                        /* temp /= 4, because 4 == 2² */
    }

    seq_t so = 0, se = 0;
    int prev = digits[len - 1];
    int run_len = 1;

    /* process digits from most to least significant */
    for (int i = len - 2; i >= 0; --i) {
        int cur = digits[i];
        if (cur == prev) {
            ++run_len;
        } else {
            /* finish previous run */
            if (run_len & 1)   /* odd length run */
                so += run_len;
            else
                se += run_len;
            /* start new run */
            prev = cur;
            run_len = 1;
        }
    }

    /* final run */
    if (run_len & 1)
        so += run_len;
    else
        se += run_len;

    return so - se;
}
