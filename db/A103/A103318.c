/*
 * A103318  Number of solutions i in range [0,n-1] to i == 0 mod 2^(n-i).
 * https://oeis.org/A103318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of i in [0, n-1] such that i ≡ 0 (mod 2^(n-i))
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           (i.e., 2^(n-i) divides i).  i = 0 is always a solution. */
 
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 for the OEIS offset */
    seq_t count = 1;                /* i = 0 is always a solution */

    /* Check all i >= 1 up to n-1 */
    for (idx_t i = 1; i < n; ++i)
    {
        idx_t exp = n - i;          /* exponent of 2 to test */

        /* For exp >= 64, 2^exp exceeds any 64‑bit i < n, so skip */
        if (exp >= 64)
            continue;

        /* 2^exp fits in a 64‑bit value */
        if ((i & ((1ULL << exp) - 1)) == 0)
            ++count;
    }

    return count;
}
