/*
 * A229337  Sum of products of elements of nonempty subsets of divisors of n.
 * https://oeis.org/A229337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{d|n} (d+1) - 1
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of products of elements of all non‑empty subsets of the divisors of n. */
#include <stdlib.h>
#include <stdint.h>

/* Simple trial‑division prime factorisation.
 * On return *cnt equals the number of distinct prime factors of n,
 * primes[i] is the i‑th prime, exps[i] its exponent. */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    if (n <= 1) {
        *cnt = 0;
        return;
    }
    /* factor 2 */
    if ((n & 1ULL) == 0) {
        size_t e = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            e++;
        }
        pr[c] = 2ULL;
        ex[c] = e;
        c++;
    }
    /* odd factors */
    for (uint64_t p = 3ULL; p * p <= n; p += 2ULL) {
        if (n % p == 0) {
            size_t e = 0;
            while (n % p == 0) {
                n /= p;
                e++;
            }
            pr[c] = p;
            ex[c] = e;
            c++;
        }
    }
    if (n > 1) {
        pr[c] = n;
        ex[c] = 1;
        c++;
    }
    *cnt = c;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0; /* position 0 not used in the OEIS offset */
    }

    uint64_t primes[64], exps[64];
    size_t pfCnt = 0;
    factor_uint64((uint64_t)n, primes, exps, &pfCnt);

    /* total number of divisors */
    size_t dcnt = 1;
    for (size_t i = 0; i < pfCnt; ++i) {
        dcnt *= (size_t)(exps[i] + 1);
    }

    /* allocate array for all divisors */
    seq_t *divisors = (seq_t *)malloc(sizeof(seq_t) * dcnt);
    if (!divisors) {
        return 0; /* unlikely for the input range */
    }

    /* enumerate divisors */
    size_t total = 1;
    divisors[0] = (seq_t)1;
    for (size_t i = 0; i < pfCnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];
        size_t cur_total = total;
        seq_t powp = (seq_t)1;
        for (uint64_t k = 0; k < e; ++k) {
            powp *= p;                       /* p^(k+1) */
            for (size_t j = 0; j < cur_total; ++j) {
                divisors[total++] = divisors[j] * powp;
            }
        }
    }

    /* product of (divisor + 1) over all divisors */
    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < total; ++i) {
        prod *= (divisors[i] + (seq_t)1);
    }

    free(divisors);
    return prod - (seq_t)1; /* subtract the empty subset contribution */
}
