/*
 * A163457  a(n) = the smallest divisor of n such that this and all greater
 *          divisors of n are non-coprime to each other.
 * https://oeis.org/A163457
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A163457: the smallest divisor d of n such that all divisors
   ≥ d are pairwise non‑coprime. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    seq_t val;      /* divisor value */
    uint64_t mask;  /* bitmask of distinct prime factors of this divisor */
} Div;

static int cmpDiv(const void *a, const void *b)
{
    const Div *A = (const Div*)a;
    const Div *B = (const Div*)b;
    if (A->val < B->val) return -1;
    if (A->val > B->val) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;   /* offset starts at n = 2 */

    /* factor n to obtain distinct prime factors */
    idx_t temp = n;
    uint64_t primes[64];
    size_t prime_cnt = 0;

    if (temp % 2 == 0) {
        primes[prime_cnt++] = 2;
        while (temp % 2 == 0) temp /= 2;
    }
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            primes[prime_cnt++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1)
        primes[prime_cnt++] = temp;

    /* collect all divisors of n (maximum < 200 for n <= 10000) */
    Div divs[200];
    size_t cnt = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            uint64_t mask = 0;
            for (size_t i = 0; i < prime_cnt; ++i)
                if (d % primes[i] == 0)
                    mask |= (1ULL << i);
            divs[cnt].val = (seq_t)d;
            divs[cnt].mask = mask;
            ++cnt;

            idx_t d2 = n / d;
            if (d2 != d) {
                mask = 0;
                for (size_t i = 0; i < prime_cnt; ++i)
                    if (d2 % primes[i] == 0)
                        mask |= (1ULL << i);
                divs[cnt].val = (seq_t)d2;
                divs[cnt].mask = mask;
                ++cnt;
            }
        }
    }

    /* sort divisors in ascending order */
    qsort(divs, cnt, sizeof(Div), cmpDiv);

    /* find the smallest divisor such that all larger divisors are pairwise
       non‑coprime (i.e., their pairwise GCD > 1) */
    for (size_t i = 0; i < cnt; ++i) {
        bool ok = true;
        for (size_t j = i; ok && j < cnt; ++j) {
            for (size_t k = j + 1; k < cnt; ++k) {
                if ((divs[j].mask & divs[k].mask) == 0) {
                    ok = false;
                    break;
                }
            }
        }
        if (ok)
            return divs[i].val;
    }

    /* Should never happen for n >= 2 */
    return 0;
}
