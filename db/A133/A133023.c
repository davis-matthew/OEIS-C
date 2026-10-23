/*
 * A133023  Divisors of 2001.
 * https://oeis.org/A133023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A133023: Divisors of 2001, sorted ascending.
   2001 = 3 × 23 × 29, so every divisor is a product of a subset of
   these three primes. The function pre‑computes the eight
   divisors once and returns the nth (n ≥ 1).  For n > 8 it returns 0. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined below offset */
    if (n > 8) return (seq_t)0;   /* behaviour outside range is irrelevant */

    static bool init = false;
    static seq_t divs[8];

    if (!init) {
        /* 2001 = 3 * 23 * 29 */
        const seq_t primes[3] = { (seq_t)3, (seq_t)23, (seq_t)29 };

        for (idx_t mask = 0; mask < 8; ++mask) {
            seq_t prod = (seq_t)1;
            for (int i = 0; i < 3; ++i)
                if (mask & (1 << i))
                    prod *= primes[i];
            divs[mask] = prod;
        }

        /* simple insertion sort on the 8 values */
        for (int i = 0; i < 8; ++i)
            for (int j = i + 1; j < 8; ++j)
                if (divs[i] > divs[j]) {
                    seq_t tmp = divs[i];
                    divs[i] = divs[j];
                    divs[j] = tmp;
                }

        init = true;
    }

    return divs[n - 1];
}
