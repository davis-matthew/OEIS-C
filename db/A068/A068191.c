/*
 * A068191  Numbers n such that A067734(n)=0; complement of A002473; at least
 *          one prime-factor of n is larger than 7, it has 2 decimal digits.
 * https://oeis.org/A068191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 179, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9135 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: all positive integers that have at least one prime factor larger than 7
   (equivalently, the complement of the 7‑smooth numbers).  For the OEIS
   numbering a(1) = 11. */
#include <stdbool.h>

/* Checks whether a number has a prime factor greater than 7. */
static bool has_factor_gt7(idx_t x)
{
    if (x < 2) return false;
    /* Divide out all factors 2,3,5,7. */
    while ((x & 1U) == 0)       x /= 2;   /* x%2==0 */
    while (x % 3 == 0)          x /= 3;
    while (x % 5 == 0)          x /= 5;
    while (x % 7 == 0)          x /= 7;
    /* If something remains, it must be a prime >7. */
    return x > 1;
}

seq_t a(idx_t n)
{
    idx_t count     = 0;
    idx_t candidate = 0;          /* start from 1 and increment */

    while (count < n) {
        ++candidate;                          /* candidate = 1,2,3,... */
        if (has_factor_gt7(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;      /* cast to the requested type */
        }
    }
    /* Never reached for valid inputs. */
    return 0;
}
