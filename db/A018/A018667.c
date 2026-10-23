/*
 * A018667  Divisors of 825.
 * https://oeis.org/A018667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 825 in increasing order (OEIS A018667)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Return the nth divisor (1-indexed) of 825; for n>12 returns 0 */
seq_t a(idx_t n)
{
    const seq_t N = (seq_t)825;
    /* Factor N */
    idx_t primes[8];
    int   exps[8];
    int   pcnt = 0;
    seq_t temp = N;

    for (idx_t i = 2; (seq_t)i * (seq_t)i <= temp; ++i) {
        if (temp % (seq_t)i == 0) {
            primes[pcnt] = i;
            int e = 0;
            while (temp % (seq_t)i == 0) {
                temp /= (seq_t)i;
                ++e;
            }
            exps[pcnt++] = e;
        }
    }
    if (temp > 1) {
        primes[pcnt] = (idx_t)temp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* Generate all divisors from the prime factorization */
    seq_t divisors[256];
    int dcnt = 1;
    divisors[0] = (seq_t)1;

    for (int k = 0; k < pcnt; ++k) {
        seq_t p = (seq_t)primes[k];
        int   e = exps[k];
        int   current_dcnt = dcnt;
        for (int ek = 1; ek <= e; ++ek) {
            seq_t mult = (seq_t)1;
            for (int t = 0; t < ek; ++t) mult *= p;
            for (int j = 0; j < current_dcnt; ++j)
                divisors[dcnt++] = divisors[j] * mult;
        }
    }

    /* Sort divisors in increasing order (bubble sort, works for 12 elements) */
    for (int i = 0; i < dcnt - 1; ++i) {
        for (int j = i + 1; j < dcnt; ++j) {
            if (divisors[i] > divisors[j]) {
                seq_t tmp = divisors[i];
                divisors[i] = divisors[j];
                divisors[j] = tmp;
            }
        }
    }

    if (n >= (idx_t)1 && n <= (idx_t)dcnt)
        return divisors[(int)n - 1];
    return (seq_t)0;
}
