/*
 * A018606  Divisors of 714.
 * https://oeis.org/A018606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth smallest positive divisor of 714
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 714 = 2 * 3 * 7 * 17, all primes appear once. */
    /* Generate all 2^4 = 16 divisors, sort them, and index. */
    if (n < 1) return 0;             /* out of bounds protection */

    /* Temporary array for divisors.  Size 16 is fixed. */
    seq_t divs[16];
    const uint64_t primes[4] = {2ULL, 3ULL, 7ULL, 17ULL};

    for (int mask = 0; mask < 16; ++mask) {
        seq_t prod = 1;  /* start with multiplicative identity */
        for (int i = 0; i < 4; ++i) {
            if (mask & (1 << i))
                prod *= (seq_t)primes[i];
        }
        divs[mask] = prod;
    }

    /* Simple insertion sort (small array, negligible cost). */
    for (int i = 0; i < 16; ++i) {
        for (int j = i + 1; j < 16; ++j) {
            if (divs[j] < divs[i]) {
                seq_t tmp = divs[i];
                divs[i] = divs[j];
                divs[j] = tmp;
            }
        }
    }

    if (n > 16) return 0;  /* sequence has only 16 terms */
    return divs[(size_t)n - 1];
}
