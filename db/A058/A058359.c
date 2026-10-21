/*
 * A058359  Absolute value of difference between the even and odd first
 *          differences of the divisors of n.
 * https://oeis.org/A058359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: For n ≥ 1, list the divisors d1 < d2 < … < dk of n.
   Compute the first differences diff_i = d_{i+1} – d_i (i = 1 … k‑1).
   Let S_even = Σ diff_i over all even diff_i, S_odd = Σ diff_i over all odd diff_i.
   Output |S_even – S_odd|. */
#include <stdint.h>
#include <stdlib.h>

/* comparison function for qsort */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t u = *(const uint64_t *)a;
    uint64_t v = *(const uint64_t *)b;
    return (u > v) - (u < v);
}

/* a(n) as defined above */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;          /* only divisor 1 → no differences */

    /* prime factorization of n */
    uint64_t tmp = (uint64_t)n;
    uint64_t primes[64];
    int exps[64];
    int pc = 0;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pc] = p;
            exps[pc] = e;
            ++pc;
        }
    }
    if (tmp > 1) {                         /* remainder is a prime */
        primes[pc] = tmp;
        exps[pc] = 1;
        ++pc;
    }

    /* generate all divisors */
    uint64_t divs[1024];
    size_t divcnt = 1;
    divs[0] = 1;
    for (int i = 0; i < pc; ++i) {
        uint64_t p = primes[i];
        int e = exps[i];
        size_t oldcnt = divcnt;
        uint64_t factor = 1;
        for (int j = 1; j <= e; ++j) {
            factor *= p;
            for (size_t k = 0; k < oldcnt; ++k) {
                divs[divcnt++] = divs[k] * factor;
            }
        }
    }

    /* sort divisors */
    qsort(divs, divcnt, sizeof(uint64_t), cmp_uint64);

    /* sum even and odd first differences */
    uint64_t sum_even = 0;
    for (size_t i = 1; i < divcnt; ++i) {
        uint64_t diff = divs[i] - divs[i - 1];
        if ((diff & 1ULL) == 0) sum_even += diff;
    }
    uint64_t total_diff = ((uint64_t)n) - 1ULL;
    uint64_t sum_odd = total_diff - sum_even;
    uint64_t abs_diff = (sum_even >= sum_odd) ? (sum_even - sum_odd) : (sum_odd - sum_even);

    return (seq_t)abs_diff;
}
