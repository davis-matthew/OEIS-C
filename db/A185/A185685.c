/*
 * A185685  Irregular triangle, read by rows, of primes with prefix n and
 *          digits "7" appended, otherwise 0.
 * https://oeis.org/A185685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less, tabf
 * Formula:   a(n) is built row‑by‑row. For each integer r ≥ 1 generate the
 *            numbers r*10+7, (r*10+7)*10+7, … . All primes encountered before
 *            the first composite in the row are output in order. If a row
 *            yields no primes, a single 0 is output
 * Range:     u8 n <= 6, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t count = 0;                          /* number of terms produced so far */
    unsigned __int128 max_seq = (unsigned __int128)((seq_t)~0); /* maximal representable value */

    for (idx_t row = 1; ; ++row)
    {
        unsigned __int128 cur = row;          /* start generating from the row number */
        bool any_prime = false;               /* whether a prime has been produced in this row */
        bool overflow = false;

        while (true)
        {
            cur = cur * 10 + 7;               /* append a digit 7 */

            if (cur > max_seq)
            {
                overflow = true;              /* number cannot be represented in seq_t */
                break;
            }

            seq_t num = (seq_t)cur;

            if (is_prime_big(num))
            {
                any_prime = true;
                ++count;
                if (count == n)
                    return num;               /* prime found */
                /* continue searching for primes in this row */
            }
            else /* composite encountered – row ends */
            {
                if (!any_prime)
                {
                    ++count;
                    if (count == n)
                        return (seq_t)0;        /* row had no primes */
                }
                break;                       /* move to the next row */
            }
        }

        if (overflow && !any_prime)
        {
            /* row exceeded representable range and produced no primes */
            ++count;
            if (count == n)
                return (seq_t)0;
        }
        /* otherwise proceed to the next row */
    }
}
