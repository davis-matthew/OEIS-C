/*
 * A166121  Let dsf(n) = n_1^{n_1}+n_2^{n_2}+n_3^{n_3} + n_m^{n_m}, where
 *          {n_1,n_2,n_3,...n_m} is the list of the digits of an integer n.
 *          dsf(791621579) = 776537851 and dsf(776537851) = 19300779, ...,
 *          dsf(824599) = 791621579, ... in this way these 11 numbers make a
 *          loop for the function dsf.
 * https://oeis.org/A166121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A166121
 *  a(1) = 791621579
 *  a(n) = dsf(a(n-1))      for n>1
 *  where dsf(x) = Σ d^d  over the decimal digits d of x
 *  and 0^0 is defined to be 1.
 *  The sequence eventually enters a fixed 11‑term loop.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/get_digits.h"

static seq_t dsf(seq_t x)
{
    uint8_t digits[32];
    size_t dlen;
    get_digits(x, digits, &dlen);

    seq_t sum = 0;
    for (size_t i = 0; i < dlen; ++i) {
        int d = digits[i];
        if (d == 0) {
            sum += 1;          /* 0^0 = 1 */
            continue;
        }
        seq_t p = 1;
        for (int j = 0; j < d; ++j)
            p *= (seq_t)d;
        sum += p;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* 11‑term null‑cycle for this sequence. */
    static seq_t cycle[11];
    static int ready = 0;
    if (!ready) {
        const seq_t init_val = (seq_t)791621579U;
        cycle[0] = init_val;
        for (int i = 1; i < 11; ++i)
            cycle[i] = dsf(cycle[i-1]);
        ready = 1;
    }
    /* wrap around the 11‑term loop */
    idx_t idx = (n - 1) % 11;
    return cycle[idx];
}
