/*
 * A018498  Divisors of 520.
 * https://oeis.org/A018498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A018498 - Divisors of 520
 *  Formula:  a(n) = n-th smallest divisor of 520; a(n)=0 for n>16
 *
 *  The sequence is derived from the prime factorisation 520 = 2^3·5·13.
 *  All divisors are generated once, sorted, and then indexed.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state to avoid recomputing divisors on every call */
    static seq_t divs[32];
    static size_t div_count = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Determine prime factorisation of 520 */
        uint64_t m = 520;
        uint64_t primes[10];
        int exps[10];
        int pc = 0;               /* number of distinct primes */

        for (uint64_t p = 2; p * p <= m; ++p) {
            if (m % p == 0) {
                primes[pc] = p;
                int e = 0;
                while (m % p == 0) {
                    m /= p;
                    ++e;
                }
                exps[pc++] = e;
            }
        }
        if (m > 1) {              /* remaining prime factor */
            primes[pc] = m;
            exps[pc++] = 1;
        }

        /* Generate all divisors from the prime exponents */
        divs[0] = 1;
        div_count = 1;
        for (int i = 0; i < pc; ++i) {
            size_t prev_cnt = div_count;
            uint64_t base = 1;
            for (int e = 1; e <= exps[i]; ++e) {
                base *= (uint64_t)primes[i];
                for (size_t j = 0; j < prev_cnt; ++j) {
                    divs[div_count++] = (seq_t)(divs[j] * base);
                }
            }
        }

        /* Sort the divisor list in ascending order (insertion sort) */
        for (size_t i = 1; i < div_count; ++i) {
            seq_t key = divs[i];
            size_t j = i;
            while (j > 0 && divs[j - 1] > key) {
                divs[j] = divs[j - 1];
                --j;
            }
            divs[j] = key;
        }

        initialized = true;
    }

    if (n == 0 || n > div_count) {
        return 0;      /* undefined or only defined up to the number of divisors */
    }

    return divs[n - 1];
}
