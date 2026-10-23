/*
 * A162764  Minimal total number of floors that an elevator must move to get n
 *          persons waiting, respectively, on floors i = 1, 2, ..., n, to
 *          their destination floors n-i+1 (= n, n-1, ..., 1), if the elevator
 *          can hold up to C = 4 persons at a time and starts at floor 1, and
 *          no passenger may get off the elevat...
 * https://oeis.org/A162764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2n-2 for n < 10; a(10)=22; a(11)=26; a(n) = 2n+2 + a(n-8)
 *            for all n ≥ 12 (conjectured to be exact)
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the small cases directly */
    if (n == 1)
        return (seq_t)0;
    if (n < 10)          /* 2n-2 for n = 2 … 9 */
        return (seq_t)(2 * n - 2);
    if (n == 10)
        return (seq_t)22;
    if (n == 11)
        return (seq_t)26;

    /* n ≥ 12 : use the recurrence a(n)=2n+2 + a(n-8) */
    seq_t sum = 0;
    idx_t m   = n;

    while (1) {
        /* add the 2m+2 term for the current n ≥ 12 */
        sum += (seq_t)(2 * m + 2);
        m   -= 8;                      /* shift to the previous block */

        /* now handle the falling‐to‑base case */
        if (m < 10)                    /* a(m)=2m-2 for 2≤m≤9 */
            sum += (seq_t)(2 * m - 2);
        else if (m == 10)              /* special value */
            sum += (seq_t)22;
        else if (m == 11)              /* special value */
            sum += (seq_t)26;
        else
            continue;                  /* m ≥ 12, loop again */

        break;                         /* base case reached */
    }

    return sum;
}
