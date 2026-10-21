/*
 * A069845  Number of distinct primes obtained by inserting a 4 at all
 *          possible places in n. This includes prefixing.
 * https://oeis.org/A069845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of distinct primes obtained by inserting a decimal
 *            digit 4 at every possible position (including before the first
 *            digit) in the decimal representation of n
 * Range:     u8 n <= 9851, u16 n <= 9851, u32 n <= 9851, u64 n <= 9851,
 *            u128 n <= 9851, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/isPrime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset starts at 1 */

    /* Count decimal digits of n */
    idx_t  tmp = n;
    int    len = 0;
    while (tmp) { tmp /= 10; ++len; }
    if (len == 0) len = 1;        /* for safety, though n≠0 here */

    /* Pre‑compute powers of ten up to len+1 */
    idx_t  pow10[len + 2];
    pow10[0] = 1;
    for (int i = 1; i <= len + 1; ++i)
        pow10[i] = pow10[i - 1] * 10;

    /* Store distinct primes found */
    idx_t primes[30];
    size_t pcount = 0;

    /* Generate numbers by inserting a 4 at every position */
    for (int pos = 0; pos <= len; ++pos) {
        /* high = leading part of n before the insertion point,
           low  = trailing part after the insertion point */
        int   shift   = len - pos;          /* number of digits moved */
        idx_t high = n / pow10[shift];
        idx_t low  = n % pow10[shift];

        /* Construct the new number with the inserted '4' */
        idx_t inserted = high * pow10[shift + 1]
                        + 4    * pow10[shift]
                        + low;

        if (isPrime(inserted)) {
            /* Check if this prime has already been seen */
            int already = 0;
            for (size_t j = 0; j < pcount; ++j)
                if (primes[j] == inserted) { already = 1; break; }

            if (!already)
                primes[pcount++] = inserted;
        }
    }

    return (seq_t)pcount;
}
