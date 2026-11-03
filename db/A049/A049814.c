/*
 * A049814  a(n)=number of Farey fractions of order n that are <=1/10; cf.
 *          A049805.
 * https://oeis.org/A049814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{b=2..n} #{ a | 1 ≤ a ≤ ⌊b/10⌋ , gcd(a,b)=1 } (the
 *            0/1 fraction is always included)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute the number of integers ≤ m that are coprime to q using inclusion–exclusion. */
static seq_t coprime_leq(idx_t q, idx_t m)
{
    if (m == 0) return 0;

    /* Factor q into distinct primes (simple trial division, q ≤ 10000). */
    uint32_t primes[12];
    size_t pcnt = 0;
    idx_t tmp = q;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            primes[pcnt++] = (uint32_t)p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) primes[pcnt++] = (uint32_t)tmp;

    seq_t sum = (seq_t)m;
    unsigned int subsets = 1U << pcnt;          /* 2^pcnt */
    for (unsigned int mask = 1; mask < subsets; ++mask) {
        idx_t prod = 1;
        int bits = 0;
        for (size_t i = 0; i < pcnt; ++i) {
            if (mask & (1U << i)) {
                prod *= (idx_t)primes[i];
                ++bits;
            }
        }
        seq_t term = (seq_t)(m / prod);
        if (bits & 1)
            sum -= term;
        else
            sum += term;
    }
    return sum;
}

/* Include definitions of seq_t and idx_t (provided by the build). */

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    seq_t res = 1; /* count the 0/1 fraction */
    for (idx_t b = 2; b <= n; ++b) {
        idx_t m = b / 10;          /* floor(b/10) */
        if (m == 0) continue;
        res += coprime_leq(b, m);
    }
    return res;
}
