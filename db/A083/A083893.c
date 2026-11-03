/*
 * A083893  Number of divisors of n with largest digit = 6 (base 10).
 * https://oeis.org/A083893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9648 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts, for n ≥ 1, the divisors of n whose largest decimal digit is exactly 6.
   The algorithm factorises n, generates all divisors recursively and counts
   those meeting the digit condition. */

#include <stdint.h>
#include <stdbool.h>

/* test whether the largest decimal digit of v is exactly 6 */
static bool largest_digit_is_six(uint64_t v)
{
    int maxd = 0;
    while (v > 0) {
        int d = v % 10;
        if (d > maxd) {
            maxd = d;
            if (maxd > 6) return false;          /* early exit */
        }
        v /= 10;
    }
    return maxd == 6;
}

/* recursive enumeration of all divisors of n.
   depth      … current prime index
   acc        … product of chosen exponents so far
   primes     … list of distinct prime factors of n
   exps       … corresponding exponents
   pfcnt      … number of distinct primes
   cnt        … pointer to result counter (seq_t)                     */
static void recurse_divisors(int depth,
                             uint64_t acc,
                             const uint64_t primes[],
                             const int exps[],
                             int pfcnt,
                             seq_t *cnt)
{
    if (depth == pfcnt) {
        if (largest_digit_is_six(acc))
            (*cnt)++;
        return;
    }

    uint64_t p = primes[depth];
    uint64_t prod = 1;
    for (int e = 0; e <= exps[depth]; ++e) {
        recurse_divisors(depth + 1, acc * prod, primes, exps, pfcnt, cnt);
        prod *= p;
    }
}

/* public function required by the build */
seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* undefined for n = 0 */

    seq_t count = 0;

    /* factorise n into distinct primes and exponents */
    uint64_t temp = n;
    uint64_t primes[64];
    int exps[64];
    int pfcnt = 0;

    /* factor 2 */
    int e = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++e;
    }
    if (e > 0) {
        primes[pfcnt] = 2;
        exps[pfcnt] = e;
        ++pfcnt;
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pfcnt] = p;
            exps[pfcnt] = e;
            ++pfcnt;
        }
    }

    /* final prime factor, if any */
    if (temp > 1) {
        primes[pfcnt] = temp;
        exps[pfcnt] = 1;
        ++pfcnt;
    }

    /* enumerate all divisors and count those with largest digit 6 */
    recurse_divisors(0, 1ULL, primes, exps, pfcnt, &count);

    return count;
}
