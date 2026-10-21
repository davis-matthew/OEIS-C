/*
 * A086222  a(n) = card{ (x,y,z) | x <= y <= z and lcm(x,y,z) = n }.
 * https://oeis.org/A086222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |{ (x,y,z) | 1 ≤ x ≤ y ≤ z ≤ n, lcm(x,y,z) = n }|
 *            Algorithm: factor n, generate all its divisors, then enumerate
 *            all non‑decreasing triples of divisors and count those whose LCM
 *            equals n. Works for any seq_t (integer type) and for all n ≥ 1
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Euclidean gcd for uint64_t */
static inline uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* lcm(a,b) = a / gcd(a,b) * b   (fits in 64 bits for n ≤ 10 000) */
static inline uint64_t lcm64(uint64_t a, uint64_t b)
{
    return a / gcd64(a, b) * b;
}

/* Comparison function for qsort (ascending order) */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t lhs = *(const uint64_t *)a;
    uint64_t rhs = *(const uint64_t *)b;
    return (lhs < rhs) - (lhs > rhs);
}

/* Recursive generation of all divisors of n from its prime factorisation */
static void gen_divs_rec(uint64_t cur,
                         size_t idx, size_t cnt,
                         const uint64_t *pr,
                         const int     *exp,
                         uint64_t      *divs,
                         size_t        *len)
{
    if (idx == cnt) {
        divs[(*len)++] = cur;
        return;
    }
    uint64_t val = cur;
    for (int e = 0; e <= exp[idx]; ++e) {
        gen_divs_rec(val, idx + 1, cnt, pr, exp, divs, len);
        val *= pr[idx];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* out of offset – defined as 0 */

    /* Factorise n (n ≤ 10000 – simple trial division is sufficient) */
    uint64_t primes[16];
    int     exps[16];
    size_t   cnt = 0;

    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (m > 1) {               /* remaining prime factor */
        primes[cnt] = m;
        exps[cnt] = 1;
        ++cnt;
    }

    /* Generate all divisors of n */
    uint64_t divs[2048];
    size_t len = 0;
    gen_divs_rec(1, 0, cnt, primes, exps, divs, &len);

    /* Sort divisors for non‑decreasing triple enumeration */
    qsort(divs, len, sizeof(uint64_t), cmp_uint64);

    /* Count triples with lcm == n */
    uint64_t target = (uint64_t)n;
    uint64_t total = 0;

    for (size_t i = 0; i < len; ++i) {
        for (size_t j = i; j < len; ++j) {
            uint64_t l12 = lcm64(divs[i], divs[j]);
            for (size_t k = j; k < len; ++k) {
                uint64_t l123 = lcm64(l12, divs[k]);
                if (l123 == target) {
                    ++total;
                }
            }
        }
    }

    return (seq_t)total;
}
