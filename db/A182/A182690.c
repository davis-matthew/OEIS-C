/*
 * A182690  a(n) = the largest 2-digit number with exactly n divisors, a(n) =
 *          0 if no such number exists.
 * https://oeis.org/A182690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the largest two‑digit number with exactly n divisors.
   Return 0 if no such number exists. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t best[13];          /* indices 0..12 */

    if (!initialized) {
        /* initialise to zero */
        for (int i = 0; i < 13; ++i)
            best[i] = 0;

        /* examine all two‑digit numbers */
        for (int num = 10; num <= 99; ++num) {
            int d = 0;
            for (int i = 1; i * i <= num; ++i) {
                if (num % i == 0) {
                    ++d;                /* divisor i */
                    if (i != num / i)
                        ++d;            /* paired divisor */
                }
            }
            if (d <= 12 && (seq_t)num > best[d])
                best[d] = (seq_t)num;
        }
        initialized = true;
    }

    if (n > 12)      /* no two‑digit number has more than 12 divisors */
        return 0;
    return best[(int)n];
}
