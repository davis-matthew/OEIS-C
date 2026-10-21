/*
 * A083892  Number of divisors of n with largest digit = 5 (base 10).
 * https://oeis.org/A083892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of divisors of n whose largest decimal digit
 *            equals 5
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* helper: largest decimal digit of x */
static int largest_digit(uint64_t x)
{
    int maxd = 0;
    while (x) {
        int d = x % 10;
        if (d > maxd) maxd = d;
        if (maxd == 9) return maxd;   /* cannot increase further */
        x /= 10;
    }
    return maxd;
}

/* recursive enumeration of divisors */
static void enumerate_divisors(const uint64_t *primes, const int *exps,
                               int cnt, int idx, uint64_t cur, seq_t *cnt5)
{
    if (idx == cnt) {
        if (largest_digit(cur) == 5) (*cnt5)++;
        return;
    }
    uint64_t mult = 1;
    for (int e = 0; e <= exps[idx]; ++e) {
        enumerate_divisors(primes, exps, cnt, idx + 1, cur * mult, cnt5);
        mult *= primes[idx];
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* factorisation of n */
    uint64_t primes[8];
    int exps[8];
    int cnt = 0;

    uint64_t temp = (uint64_t)n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            cnt++;
        }
    }
    if (temp > 1) {
        primes[cnt] = temp;
        exps[cnt] = 1;
        cnt++;
    }

    seq_t result = 0;
    enumerate_divisors(primes, exps, cnt, 0, 1, &result);
    return result;
}
