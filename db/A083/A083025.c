/*
 * A083025  Number of primes congruent to 1 modulo 4 dividing n (with
 *          multiplicity).
 * https://oeis.org/A083025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = number of prime divisors of n that are ≡ 1 (mod 4),
 *            counted with multiplicity. a(1)=0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Small trial‑division factoriser for 64‑bit numbers.
   pr   : array of primes found (size ≥ 64)
   exp  : array of exponents (size ≥ 64)
   cnt  : number of distinct prime factors returned
   The largest possible number of distinct prime factors of a 64‑bit
   integer is less than 64. */
static void factor_uint64(uint64_t n, uint64_t *pr, uint8_t *exp, size_t *cnt)
{
    size_t i = 0;
    /* factor 2 */
    if ((n & 1ULL) == 0) {
        pr[i] = 2ULL;
        exp[i] = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            exp[i]++;
        }
        i++;
    }
    /* odd primes */
    for (uint64_t p = 3ULL; p * p <= n; p += 2ULL) {
        if (n % p == 0) {
            pr[i] = p;
            exp[i] = 0;
            while (n % p == 0) {
                n /= p;
                exp[i]++;
            }
            i++;
        }
    }
    /* remaining prime */
    if (n > 1ULL) {
        pr[i] = n;
        exp[i] = 1;
        i++;
    }
    *cnt = i;
}

seq_t a(idx_t n)
{
    /* a(1) and any index below 1 are defined as 0 */
    if (n <= 1u)
        return 0;

    uint64_t nn   = (uint64_t)n;
    uint64_t pr[64];
    uint8_t  exp[64];
    size_t   cnt;

    factor_uint64(nn, pr, exp, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if ((pr[i] & 3u) == 1u)   /* pr[i] % 4 == 1 */
            sum += (seq_t)exp[i];
    }
    return sum;
}
