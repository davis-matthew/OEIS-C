/*
 * A027892  Divisors of 999999.
 * https://oeis.org/A027892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th smallest divisor of 999999
 * Range:     u8 n <= 1, u16 n <= 46, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: 999999 = 3^3 * 7 * 11 * 13 * 37, so there are 64 divisors. */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset < 1 */

    /* Prime factors and exponents of 999999 */
    const uint64_t primes[] = {3, 7, 11, 13, 37};
    const uint8_t  exps[]   = {3, 1, 1, 1, 1};
    const size_t   num_primes = 5;

    /* Buffer for all divisors (max 64) */
    seq_t divs[64];
    size_t cnt = 1;
    divs[0] = (seq_t)1;

    /* Generate all divisors (unsorted) */
    for (size_t pi = 0; pi < num_primes; ++pi) {
        uint64_t p = primes[pi];
        uint8_t  e = exps[pi];

        size_t base_cnt = cnt;
        seq_t  p_pow = (seq_t)1;
        for (uint8_t k = 1; k <= e; ++k) {
            p_pow = p_pow * (seq_t)p;            /* p^k */
            for (size_t j = 0; j < base_cnt; ++j)
                divs[cnt++] = divs[j] * p_pow;
        }
    }

    /* Simple insertion sort – 64 elements is trivial */
    for (size_t i = 1; i < cnt; ++i) {
        seq_t key = divs[i];
        size_t j = i;
        while (j > 0 && divs[j - 1] > key) {
            divs[j] = divs[j - 1];
            --j;
        }
        divs[j] = key;
    }

    /* Return the nth divisor (1‑based). For n > 64 return 0. */
    if ((idx_t)(n - 1) < cnt)
        return divs[n - 1];
    return (seq_t)0;
}
