/*
 * A085821  Smallest prime > A085820(n) ending with A085820(n).
 * https://oeis.org/A085821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  full, nonn, fini, base
 * Formula:   a(n) = smallest prime > A085820(n) ending with A085820(n), where
 *            A085820(n) is the nth odd number not divisible by 5
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   digit_len, isPrimeGeneric, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"


seq_t a(idx_t n)
{
    /* Compute the nth odd number not divisible by 5
       A085820(n) = 10 * floor((n-1)/4) + {1,3,7,9}[ (n-1) mod 4 ]   */
    static const seq_t offs[4] = {1, 3, 7, 9};
    idx_t q = (n - 1) / 4;
    idx_t r = (n - 1) % 4;
    seq_t t = (seq_t)(10 * q + offs[r]);

    /* Number of decimal digits of t */
    int d = digit_len(t);

    /* 10^d */
    seq_t m = pow10((idx_t)d);

    for (idx_t k = 1; ; ++k) {
        seq_t candidate = ((seq_t)k * m) + t;  /* candidate > t */
        if (candidate >= 100 && isPrimeGeneric(candidate))
            return candidate;
    }
}
