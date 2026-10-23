/*
 * A125746  a(n) = smallest divisor d of n such that n <= {sum of d and all
 *          smaller divisors of n}.
 * https://oeis.org/A125746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest divisor d of n such that the sum of all divisors
 *            of n not exceeding d is >= n
 * Range:     u8 n <= 255, u16 n <= 9787, u32 n <= 9787, u64 n <= 9787,
 *            u128 n <= 9787, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static int cmp_uint64(const void *p1, const void *p2)
{
    uint64_t a = *(const uint64_t *)p1;
    uint64_t b = *(const uint64_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* Recursively generate all divisors from the prime factorisation */
static void gen_divisors(size_t idx, uint64_t cur,
                         const uint32_t *primes, const int *exps,
                         size_t cnt,
                         uint64_t *out, size_t *outcnt, size_t maxout)
{
    if (idx == cnt) {
        if (*outcnt < maxout) out[(*outcnt)++] = cur;
        return;
    }
    uint64_t val = cur;
    for (int e = 0; e <= exps[idx]; ++e) {
        gen_divisors(idx + 1, val, primes, exps, cnt, out, outcnt, maxout);
        val *= primes[idx];
    }
}

seq_t a(idx_t n)
{
    /* Special case: n == 0 is irrelevant (offset 1), but guard anyway */
    if (n == 0) return 0;

    /* Factor n */
    uint32_t primes[64];
    int exps[64];
    size_t cnt = 0;
    uint64_t temp = (uint64_t)n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt] = (uint32_t)p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (temp > 1) {
        primes[cnt] = (uint32_t)temp;
        exps[cnt] = 1;
        ++cnt;
    }

    /* Generate all divisors, up to a generous limit */
    uint64_t divs[1024];
    size_t divcnt = 0;
    gen_divisors(0, 1, primes, exps, cnt, divs, &divcnt, 1024);

    /* Sort the divisors */
    qsort(divs, divcnt, sizeof(uint64_t), cmp_uint64);

    /* Accumulate sums until reaching n */
    seq_t sum = 0;
    seq_t target = (seq_t)n;
    for (size_t i = 0; i < divcnt; ++i) {
        sum += (seq_t)divs[i];
        if (sum >= target) return (seq_t)divs[i];
    }

    /* Fallback: should not happen (n itself is a divisor) */
    return (seq_t)n;
}
