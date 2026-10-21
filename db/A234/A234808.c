/*
 * A234808  a(n) = |{0 < k < n: p = k + phi(n-k) and 2*n - p are both prime}|,
 *          where phi(.) is Euler's totient function.
 * https://oeis.org/A234808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{0 < k < n : p = k + phi(n - k) and 2*n - p are both
 *            prime}|
 * Range:     u8 n <= 369, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* handle trivial case */
    if (n <= 1)
        return 0;

    seq_t count = 0;
    seq_t two_n = (seq_t)n * 2;

    for (idx_t k = 1; k < n; ++k)
    {
        seq_t phi_val = phi(n - k);            /* phi(n-k) */
        seq_t p = (seq_t)k + phi_val;          /* candidate prime p */
        seq_t two_n_minus_p = two_n - p;       /* 2*n - p */

        if (isPrimeGeneric(p) && isPrimeGeneric(two_n_minus_p))
            ++count;
    }

    return count;
}
