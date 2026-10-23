/*
 * A169668  The product of factorials s! where s belongs to the multiset of
 *          exponents of the Lie groups G=Br or G=Cr. Also 2^r times the
 *          classical Lie superfactorial of type Br ~ SO(2r+1). Also
 *          2^{r(r-1)} times the Lie superfactorial of type Cr ~ Sp(2r).
 *          Duplicate of A168467.
 * https://oeis.org/A169668
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{k=1}^{n} ((2k-1)!)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 2) {
        return 0; /* a(n) is defined only for n ≥ 2 in OEIS */
    }
    seq_t result = 1;
    seq_t fact = 1;
    /* Incrementally build factorials 1! … (2n-1)!.
       Whenever the current integer i is odd, multiply the current
       factorial into the running product `result`. */
    idx_t max_i = 2*n - 1;
    for (idx_t i = 1; i <= max_i; ++i) {
        fact = fact * (seq_t)i;
        if ((i & 1) == 1) {
            result = result * fact;
        }
    }
    return result;
}
