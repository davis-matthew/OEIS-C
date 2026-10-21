/*
 * A207524  Number of rational numbers p/q such that 0<p<q<=n and
 *          p/q<=(greatest quotient of consecutive Fibonacci numbers having
 *          denominator <= n).
 * https://oeis.org/A207524
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{q=2}^{n} |{ p | 1 <= p <= ⌊2q/3⌋ and gcd(p,q)=1 }|
 *            Counts the reduced fractions p/q (0 < p < q <= n) that are <=
 *            2/3. This equals the number of rational numbers p/q with
 *            denominator up to n and value not exceeding the largest
 *            Fibonacci quotient with denominator <= n (which is 2/3 for...
 * Range:     u8 n <= 34, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Counts integers <= m that are coprime to a given number num.
   Prime factors of num are supplied via the smallest prime factor array spf.
   The array spf must contain spf[0..num] with spf[i] >= i for primes. */
static seq_t coprime_count(uint32_t num, seq_t m, const uint32_t *spf)
{
    /* collect distinct prime factors of num */
    uint32_t primes[16]; /* enough for num <= 1e12, but here num <= 10000 */
    int pc = 0;
    uint32_t tmp = num;
    while (tmp > 1) {
        uint32_t p = spf[tmp];
        primes[pc++] = p;
        while (tmp % p == 0) tmp /= p;
    }
    seq_t result = (seq_t)m;   /* start with all numbers up to m */
    int subsets = 1 << pc;
    for (int mask = 1; mask < subsets; ++mask) {
        seq_t prod = 1;
        int bits = __builtin_popcount((unsigned)mask);
        for (int i = 0; i < pc; ++i)
            if (mask & (1 << i))
                prod *= (seq_t)primes[i];
        seq_t div = m / prod;
        if (bits & 1) result -= div;   /* odd number of primes → subtract */
        else          result += div;   /* even number → add back */
    }
    return result;
}

/* Computes the nth term of sequence A207524.
   n >= 2 is assumed; behavior for n < 2 is undefined. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    /* small n: allocate SPF array */
    uint32_t *spf = (uint32_t*)malloc((size_t)(n + 1) * sizeof(uint32_t));
    if (!spf) return (seq_t)0;   /* allocation failure */
    for (uint32_t i = 0; i <= n; ++i) spf[i] = 0;
    /* sieve for smallest prime factor */
    for (uint32_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if ((uint64_t)i * i <= n) {
                for (uint32_t j = i * i; j <= n; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    seq_t total = (seq_t)0;
    for (uint32_t q = 2; q <= n; ++q) {
        seq_t m = ((seq_t)q * (seq_t)2) / (seq_t)3;   /* floor(2q/3) */
        total += coprime_count(q, m, spf);
    }

    free(spf);
    return total;
}
