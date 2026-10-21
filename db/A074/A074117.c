/*
 * A074117  Smallest n-digit power of 3.
 * https://oeis.org/A074117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 3^ceil((n-1)*log(10)/log(3))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   pow10, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* The OEIS sequence starts at index 1.  For n==0 we return 1 as a harmless
       fallback; behaviour below the offset does not matter. */
    if (n == 0)
        return 1;

    /* The target value is 10^(n-1); the smallest 3^e that is at least this. */
    seq_t target = pow10(n - 1);

    /* Use a rational over‑approximation of ln(10)/ln(3).  */
    /*  109/52 ≈ 2.0961538 > ln(10)/ln(3) ≈ 2.0959031.  */
    const idx_t NUM = 109;
    const idx_t DEN = 52;

    /* Compute ceil((n-1)*NUM/DEN) without overflowing by splitting the product. */
    idx_t n_minus = n - 1;
    idx_t e = (n_minus / DEN) * NUM
            + ((n_minus % DEN) * NUM + DEN - 1) / DEN;

    /* If the estimate is too high, decrement until 3^(e‑1) is too small. */
    while (e > 0) {
        seq_t prev = pow3(e - 1);
        if (prev < target)
            break;
        e--;
    }

    /* If the estimate is too low, increment until 3^e >= target. */
    while (1) {
        seq_t cur = pow3(e);
        if (cur >= target)
            break;
        e++;
    }

    return pow3(e);
}
