/*
 * A068640  Define f(n) = 2n+1, a(n) = largest prime of the form
 *          f(f(f(...(n))). If no such prime exists then a(n) = 0.
 * https://oeis.org/A068640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime in the sequence formed by iteratively
 *            applying f(x)=2x+1 to n until a composite is produced. If the
 *            first f(n) is composite a(n)=0
 * Range:     u8 n <= 43, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Simple trial‑division primality test for 64‑bit integers.  The chain is
   very short for n ≤ 10 000, so this is more than fast enough. */
static bool is_prime_u64_local(uint64_t n)
{
    if (n < 2)
        return false;
    if (n % 2 == 0)
        return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
    {
        if (n % d == 0)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    uint64_t cur = (uint64_t)n;  /* starting value (index) */
    seq_t best = 0;              /* best prime found so far */

    while (1)
    {
        /* Compute next value: f(cur) = 2*cur + 1 */
        if (cur > (UINT64_MAX - 1) / 2)
            break;                      /* avoid unsigned overflow */

        cur = cur * 2 + 1;

        if (is_prime_u64_local(cur))
        {
            best = (seq_t)cur;          /* store the prime */
            continue;                   /* keep going until composite */
        }
        break;                           /* composite reached */
    }

    return best;
}
