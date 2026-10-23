/*
 * A201784  Primes of the form 5n^2 - 2.
 * https://oeis.org/A201784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime of the form 5 k² – 2 (OEIS A201784)
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_seq.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    /*  n ≥ 1 is assumed (undefined for n < 1) */
    idx_t found = 0;
    idx_t k = 1;
    /* maximum value representable by seq_t, using wrap‑around cast */
    unsigned __int128 max_seq = (unsigned __int128)((seq_t)-1);

    for (;; ++k) {
        /* compute 5*k*k – 2 in a type that cannot overflow for the
           expected input range for all sane seq_t sizes */
        unsigned __int128 v = (unsigned __int128)5 * k * k - 2;

        /* stop once the value can no longer fit into seq_t */
        if (v > max_seq) break;

        seq_t val = (seq_t)v;      /* safe cast: v ≤ max_seq */
        if (is_prime_seq(val)) {
            ++found;
            if (found == n)
                return val;
        }
    }
    return 0; /* unreachable for valid n with enough representable terms */
}
