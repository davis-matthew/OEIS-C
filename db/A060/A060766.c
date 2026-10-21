/*
 * A060766  Least common multiple of differences between consecutive divisors
 *          of n (ordered by size).
 * https://oeis.org/A060766
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm of differences between consecutive divisors of n
 *            (divisors sorted increasingly, differences of successive ones)
 * Range:     u8 n <= 37, u16 n <= 441, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* comparison function for qsort of unsigned 64‑bit integers   */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va > vb) - (va < vb);
}

/* Euclidean algorithm for seq_t (works for any unsigned sized type) */
static inline seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Factorize n by trial division; store primes and their exponents.
   The caller must ensure primes[] and exp[] are large enough.  */
static void factor_uint64(uint64_t n, uint64_t *primes, int *exp, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[c] = p;
            exp[c] = e;
            ++c;
        }
    }
    if (n > 1) {          /* remaining prime factor */
        primes[c] = n;
        exp[c]   = 1;
        ++c;
    }
    *cnt = c;
}

seq_t a(idx_t n)
{
    /* n ≥ 2 for the OEIS offset; for n < 2 return 0 as placeholder */
    if (n < 2) {
        return 0;
    }

    /* factor n */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    int       exp[64];
    size_t    cnt;
    factor_uint64(nn, primes, exp, &cnt);

    /* generate all divisors (unsorted) */
    size_t capacity = 128;
    uint64_t *divs = (uint64_t *)malloc(capacity * sizeof(uint64_t));
    if (!divs) return (seq_t)0;          /* allocation failure – unlikely */

    size_t size = 1;
    divs[0] = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        int e = exp[i];
        size_t cur_size = size;
        for (size_t j = 0; j < cur_size; ++j) {
            uint64_t base = divs[j];
            uint64_t pow  = 1;
            for (int k = 1; k <= e; ++k) {
                pow *= p;
                /* ensure capacity */
                if (size == capacity) {
                    capacity *= 2;
                    uint64_t *tmp = (uint64_t *)realloc(divs, capacity * sizeof(uint64_t));
                    if (!tmp) { free(divs); return (seq_t)0; }
                    divs = tmp;
                }
                divs[size++] = base * pow;
            }
        }
    }

    /* sort divisors */
    qsort(divs, size, sizeof(uint64_t), cmp_uint64);

    /* compute lcm of successive differences */
    seq_t lcm = (seq_t)1;
    for (size_t i = 0; i + 1 < size; ++i) {
        uint64_t diff = divs[i + 1] - divs[i];
        seq_t d = (seq_t)diff;
        seq_t g = gcd_seq_t(lcm, d);
        lcm = (lcm / g) * d;
    }

    free(divs);
    return lcm;
}
