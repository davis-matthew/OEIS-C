/*
 * A018666  Divisors of 824.
 * https://oeis.org/A018666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 824: a(n) = the n‑th divisor of 824 in ascending order. */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t divisors[8];
    static int initialized = 0;
    static size_t divisor_count = 0;

    if (!initialized) {
        const idx_t N = 824;
        idx_t primes[10];
        idx_t exps[10];
        size_t primes_m = 0;
        idx_t tmp = N;

        /* factorise N */
        for (idx_t p = 2; p * p <= tmp; ++p) {
            if (tmp % p == 0) {
                idx_t cnt = 0;
                do {
                    tmp /= p;
                    cnt++;
                } while (tmp % p == 0);
                primes[primes_m] = p;
                exps[primes_m] = cnt;
                primes_m++;
            }
        }
        if (tmp > 1) {
            primes[primes_m] = tmp;
            exps[primes_m] = 1;
            primes_m++;
        }

        /* count divisors */
        divisor_count = 1;
        for (size_t i = 0; i < primes_m; ++i)
            divisor_count *= (size_t)(exps[i] + 1);

        /* generate all divisors in 64‑bit to avoid overflow */
        uint64_t raw[8];
        size_t idx = 0;

        if (primes_m == 1) {
            for (uint64_t e0 = 0; e0 <= exps[0]; ++e0) {
                uint64_t val = 1;
                for (uint64_t k = 0; k < e0; ++k) val *= primes[0];
                raw[idx++] = val;
            }
        } else if (primes_m == 2) {
            for (uint64_t e0 = 0; e0 <= exps[0]; ++e0) {
                uint64_t part0 = 1;
                for (uint64_t k = 0; k < e0; ++k) part0 *= primes[0];
                for (uint64_t e1 = 0; e1 <= exps[1]; ++e1) {
                    uint64_t val = part0;
                    for (uint64_t k = 0; k < e1; ++k) val *= primes[1];
                    raw[idx++] = val;
                }
            }
        } else {
            /* fallback: brute force (should not happen for 824) */
            for (idx_t d = 1; d <= N; ++d)
                if (N % d == 0)
                    raw[idx++] = d;
        }

        /* sort */
        for (size_t i = 0; i < idx; ++i)
            for (size_t j = i + 1; j < idx; ++j)
                if (raw[j] < raw[i]) {
                    uint64_t t = raw[i];
                    raw[i] = raw[j];
                    raw[j] = t;
                }

        /* store in final array */
        for (size_t i = 0; i < idx; ++i)
            divisors[i] = (seq_t)raw[i];

        initialized = 1;
    }

    if (n == 0)
        return (seq_t)0;

    if ((size_t)n > divisor_count)
        return (seq_t)0;

    return divisors[(size_t)n - 1];
}
