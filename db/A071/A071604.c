/*
 * A071604  a(n) is the number of 7-smooth numbers <= n.
 * https://oeis.org/A071604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of 7‑smooth numbers ≤ n (a 7‑smooth number is of
 *            the form 2^x·3^y·5^z·7^u with x,y,z,u ≥ 0)
 * Range:     u8 n <= 4499, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* local list of the four allowed primes */
static const idx_t primes[4] = {2, 3, 5, 7};

/* Recursive helper: count the 7‑smooth numbers ≤ bound
   using only the primes starting from indices idx..3. */
static seq_t cnt_smooth(idx_t bound, int idx)
{
    if (bound == 0)
        return 0;                       /* no smooth number ≤ 0 */

    if (idx >= 4)
        return 1;                       /* the only number left is 1 (always ≤ bound) */

    seq_t total = 0;
    idx_t p = primes[idx];
    idx_t power = 1;                     /* p^k , k starts at 0 */

    while (power <= bound) {
        total += cnt_smooth(bound / power, idx + 1);

        /* avoid overflow of p*power; stop if next power would exceed bound */
        if (power > bound / p)
            break;
        power *= p;
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined for n < 1, but return 0 safely */

    return cnt_smooth(n, 0);
}
