/*
 * A200723  The sum of integers k from 1 to n such that the greatest common
 *          unitary divisor of k and n is 1.
 * https://oeis.org/A200723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of k (1 ≤ k ≤ n) whose greatest common unitary
 *            divisor with n is 1. Implementation: factor n, then use
 *            inclusion–exclusion on the prime factors of n. For a subset S of
 *            primes, numbers k counted by k = M_S * m with M_S = ∏ p^e (p∈S)
 *            gcd(m, Q_S) = 1 (Q_S = ∏ p (p∈S)) The sum over such k...
 * Range:     u8 n <= 24, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* External linkage as required */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* offset is 1, 0 unused */

    /* Total sum of 1 .. n */
    uint64_t total = ((uint64_t)n * (n + 1)) / 2;
    uint64_t result = total;

    /* Factorize n into distinct primes and their prime‑power values */
    uint64_t primes[32];
    uint64_t powers[32];
    size_t cnt = 0;
    uint64_t tmp = n;
    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            uint64_t pw = 1;
            while (tmp % p == 0) {
                tmp /= p;
                pw *= p;
            }
            primes[cnt] = p;
            powers[cnt] = pw;
            ++cnt;
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        primes[cnt] = tmp;
        powers[cnt] = tmp;
        ++cnt;
    }
    if (cnt == 0)           /* n == 1 */
        return (seq_t)result;

    /* Inclusion–exclusion over subsets of the primes of n */
    uint64_t subsets = 1ULL << cnt;
    for (uint64_t mask = 1; mask < subsets; ++mask) {
        /* M_S = ∏ p^e , Q_S = ∏ p for primes in mask */
        uint64_t M = 1;
        uint64_t Q = 1;
        for (size_t i = 0; i < cnt; ++i) {
            if (mask & (1ULL << i)) {
                M *= powers[i];
                Q *= primes[i];
            }
        }
        uint64_t T = n / M;           /* floor(n / M) */

        /* Sum of m ≤ T with gcd(m, Q) = 1 */
        uint64_t sum_coprime = 0;
        uint64_t dmask = mask;
        while (1) {
            uint64_t d = 1;           /* product of primes in dmask */
            size_t bits_d = 0;
            for (size_t i = 0; i < cnt; ++i) {
                if (dmask & (1ULL << i)) {
                    d *= primes[i];
                    ++bits_d;
                }
            }
            uint64_t Tp = T / d;
            uint64_t term = d * Tp * (Tp + 1) / 2;  /* sum of multiples of d */
            if ((bits_d & 1) == 0)
                sum_coprime += term;   /* μ(d)=+1 */
            else
                sum_coprime -= term;   /* μ(d)=-1 */

            if (dmask == 0) break;
            dmask = (dmask - 1) & mask;
        }

        uint64_t sum_subset = M * sum_coprime;
        /* Sign: add if |mask| is even, subtract if odd */
        if ((__builtin_popcountll(mask) & 1) == 0)
            result += sum_subset;
        else
            result -= sum_subset;
    }

    return (seq_t)result;
}
