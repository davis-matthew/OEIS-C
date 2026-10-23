/*
 * A018612  Divisors of 725.
 * https://oeis.org/A018612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th smallest divisor of 725 (in ascending order)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This gives a(1)=1, a(2)=5, a(3)=25, a(4)=29, a(5)=145, a(6)=725. */

#include <stddef.h>
#include <stdbool.h>

/* Precomputed divisors of 725 (5^2 * 29). */
static void init_divisors(seq_t *dst, size_t *len)
{
    const idx_t primes[2] = {5, 29};
    const idx_t exps[2]  = {2, 1}; /* exponents for each prime */
    size_t c = 0;

    for (idx_t e1 = 0; e1 <= exps[0]; ++e1) {
        seq_t val1 = 1;
        for (idx_t k = 0; k < e1; ++k)
            val1 *= primes[0];
        for (idx_t e2 = 0; e2 <= exps[1]; ++e2) {
            seq_t val = val1;
            for (idx_t k = 0; k < e2; ++k)
                val *= primes[1];
            dst[c++] = val;
        }
    }

    /* simple insertion sort for 6 elements */
    for (size_t i = 1; i < c; ++i) {
        seq_t key = dst[i];
        size_t j = i;
        while (j > 0 && dst[j - 1] > key) {
            dst[j] = dst[j - 1];
            --j;
        }
        dst[j] = key;
    }
    *len = c;
}

seq_t a(idx_t n)
{
    static seq_t divisors[6];
    static size_t  div_count = 0;
    static bool    ready = false;

    if (!ready) {
        init_divisors(divisors, &div_count);
        ready = true;
    }

    if ((idx_t)div_count < n)
        return 0;          /* undefined for n > 6 */
    return divisors[n - 1];
}
