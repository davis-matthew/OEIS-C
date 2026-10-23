/*
 * A173540  Triangle read by rows in which row n lists the proper nondivisors
 *          of n, or zero if n <= 2.
 * https://oeis.org/A173540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9751, u16 n <= 9751, u32 n <= 9751, u64 n <= 9751,
 *            u128 n <= 9751, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A173540: 
   a(n) is the n‑th element of the triangle read by rows. 
   Row r lists all proper non‑divisors of r (numbers < r that do not divide r);
   for r ≤ 2 the row contains the single element 0.
*/

#include <stdint.h>
#include <stddef.h>

/* Count all positive divisors of m (including 1 and m). */
static size_t divisor_count(idx_t m)
{
    size_t cnt = 0;
    for (idx_t i = 1; i <= m / i; ++i) {
        if (m % i == 0) {
            ++cnt;                    /* divisor i */
            if (i != m / i)           /* divisor m/i, different from i */
                ++cnt;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety for out‑of‑range calls */

    idx_t r = 1;                          /* current row */
    idx_t remaining = n;                  /* how many positions left */

    /* Find the row that contains the n‑th element. */
    while (1) {
        size_t len;                        /* number of elements in this row */
        if (r <= 2) {
            len = 1;                      /* row 1 and 2 contain only 0 */
        } else {
            size_t d = divisor_count(r);
            /* proper non‑divisors count = r - d(r) */
            len = (size_t) r - d;
        }

        if (remaining <= (idx_t)len)      /* found the containing row */
            break;

        remaining -= (idx_t)len;
        ++r;
    }

    /* Special case: first two rows contain only 0. */
    if (r <= 2)
        return 0;

    /* Find the remaining‑th proper non‑divisor in row r. */
    idx_t kth = remaining;                /* 1‑based index within the row */
    for (idx_t i = 1; i < r; ++i) {
        if (r % i != 0) {                 /* i is a proper non‑divisor */
            --kth;
            if (kth == 0)
                return (seq_t)i;
        }
    }

    /* Should never reach here.  Return 0 as a safe fallback. */
    return 0;
}
