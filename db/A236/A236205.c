/*
 * A236205  For k=2,3,...,10, read in decimal base-k representation of n. a(n)
 *          is the sum of numbers of divisors of all read numbers.
 * https://oeis.org/A236205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = Σ_{k=2}^{10} τ( digits_of_n_in_base(k) interpreted as a
 *            base‑10 integer )
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"   /* provides divisor_count_u64(uint64_t) */

seq_t a(idx_t n)
{
    /* Pre‑compute powers of 10 up to 15 digits (enough for n ≤ 10000) */
    static uint64_t pow10[16] = {0};
    static bool initialized = false;
    if (!initialized) {
        pow10[0] = 1;
        for (int i = 1; i < 16; ++i) {
            pow10[i] = pow10[i-1] * 10ULL;
        }
        initialized = true;
    }

    seq_t sum = 0;   /* result accumulator */

    for (int k = 2; k <= 10; ++k) {
        uint64_t temp = (uint64_t)n;     /* copy of n to extract base‑k digits */
        uint64_t value = 0;              /* decimal value of the digit string */
        int pos = 0;                     /* current decimal place (units, tens, …) */

        /* Convert n to base‑k, simultaneously build decimal value */
        while (temp > 0) {
            uint64_t digit = temp % k;          /* digit in base k (0…k‑1, so 0…9) */
            value += digit * pow10[pos];
            ++pos;
            temp /= k;
        }

        /* Add divisor count of the constructed decimal number */
        sum += (seq_t)divisor_count_u64(value);
    }

    return sum;
}
