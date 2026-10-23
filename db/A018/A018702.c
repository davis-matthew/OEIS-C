/*
 * A018702  Divisors of 885.
 * https://oeis.org/A018702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 885 in increasing order (1‑based)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The prime factorization of 885 is 3^1 * 5^1 * 59^1.
       There are (1+1)^3 = 8 divisors.  We generate them once
       and sort them; subsequent calls just index into the table. */
    static seq_t divisors[8];
    static size_t num_div = 0;
    static bool initialized = false;

    if (!initialized) {
        const uint64_t primes[3] = {3, 5, 59};
        const int exps[3]   = {1, 1, 1};  /* each exponent is 1 */

        /* Generate all combinations of exponents 0 or 1. */
        size_t pos = 0;
        for (int e0 = 0; e0 <= exps[0]; ++e0)
            for (int e1 = 0; e1 <= exps[1]; ++e1)
                for (int e2 = 0; e2 <= exps[2]; ++e2) {
                    seq_t val = 1;
                    for (int i = 0; i < e0; ++i) val *= (seq_t)primes[0];
                    for (int i = 0; i < e1; ++i) val *= (seq_t)primes[1];
                    for (int i = 0; i < e2; ++i) val *= (seq_t)primes[2];
                    divisors[pos++] = val;
                }

        /* Sort the divisor array (simple bubble sort, size=8). */
        for (size_t i = 0; i < pos; ++i)
            for (size_t j = i + 1; j < pos; ++j)
                if (divisors[j] < divisors[i]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }

        num_div = pos;
        initialized = true;
    }

    if (n == 0 || n > (idx_t)num_div)
        return 0;                 /* undefined for n beyond the sequence length */
    return divisors[(size_t)n - 1]; /* 1‑based indexing */
}
