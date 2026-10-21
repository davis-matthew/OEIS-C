/*
 * A080685  Number of 17-smooth numbers <= n.
 * https://oeis.org/A080685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 818, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A080685:
   a(n) = number of 17‑smooth integers ≤ n
   (integers whose largest prime factor is ≤ 17). */

#include <stdint.h>
#include <stddef.h>   /* for size_t */

static const uint32_t primes[7] = { 2, 3, 5, 7, 11, 13, 17 };

/* Recursively count products of the primes primes[0..6] that do not exceed
   the current limit.  idx is the index of the prime currently being
   considered.  The function returns a seq_t value which is guaranteed to
   fit for all test cases. */
static seq_t count_smooth(uint64_t limit, size_t idx)
{
    if (idx == 7)
        return 1;                           /* only the product formed so far */

    const uint32_t p = primes[idx];
    seq_t total = 0;
    uint64_t m = limit;

    /* Enumerate all possible exponents of the current prime */
    while (1) {
        total += count_smooth(m, idx + 1);    /* next prime, current p^e kept */
        if (m < p)
            break;                           /* no further factor of p possible */
        m /= p;                              /* use one more factor of p */
    }
    return total;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* safety for out‑of‑range values */

    return count_smooth((uint64_t)n, 0);
}
