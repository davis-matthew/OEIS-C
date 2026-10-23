/*
 * A151671  G.f.: Product_{k >= 0} (1 + 3*x^(5^k)).
 * https://oeis.org/A151671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥0}(1 + 3·x^{5^k})
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For a given n, write n in base‑5.  If any digit exceeds 1 then a(n)=0; otherwise a(n)=3^{(# of 1‑digits)}. */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    idx_t m = n;
    idx_t count = 0;      /* number of 1‑digits in base‑5 representation */

    while (m > 0)
    {
        idx_t digit = m % 5;
        if (digit > 1)
            return (seq_t)0;       /* cannot represent n with distinct powers of 5 */
        count += digit;          /* digit is 0 or 1, so adding it counts the 1‑digits */
        m /= 5;
    }
    /* n == 0 yields count = 0, pow3(0) = 1 as required */
    return pow3(count);
}
