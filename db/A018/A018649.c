/*
 * A018649  Divisors of 792.
 * https://oeis.org/A018649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Sequence A018649: Divisors of 792 (sorted increasingly).
   a(n) = the n-th divisor of 792 for n = 1..24, otherwise a(n) = 0. */
seq_t a(idx_t n)
{
    /* 792 has 24 divisors.  Return 0 if n is out of bounds. */
    if (n == (idx_t)0 || n > (idx_t)24)
        return (seq_t)0;

    /* Prime factorisation of 792 = 2^3 * 3^2 * 11^1 */
    static const uint64_t primes[3] = {2ULL, 3ULL, 11ULL};
    static const int      exps[3]   = {3, 2, 1};

    /* Generate all 24 divisors. */
    seq_t divs[24];
    int   count = 0;

    for (int e0 = 0; e0 <= exps[0]; ++e0) {
        uint64_t p0 = 1;
        for (int i0 = 0; i0 < e0; ++i0) p0 *= primes[0];
        for (int e1 = 0; e1 <= exps[1]; ++e1) {
            uint64_t p1 = p0;
            for (int i1 = 0; i1 < e1; ++i1) p1 *= primes[1];
            for (int e2 = 0; e2 <= exps[2]; ++e2) {
                uint64_t p2 = p1;
                for (int i2 = 0; i2 < e2; ++i2) p2 *= primes[2];
                divs[count++] = (seq_t)p2;
            }
        }
    }

    /* Sort the 24 divisors (insertion sort is fast for such a short array). */
    for (int i = 1; i < 24; ++i) {
        seq_t key = divs[i];
        int   j   = i - 1;
        while (j >= 0 && divs[j] > key) {
            divs[j + 1] = divs[j];
            --j;
        }
        divs[j + 1] = key;
    }

    /* Return the (n-1)-th element (0‑based index). */
    return divs[(size_t)(n - 1)];
}
