/*
 * A083894  Number of divisors of n with largest digit = 7 (base 10).
 * https://oeis.org/A083894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of divisors of n with largest decimal digit = 7
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 8901 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return true if the greatest decimal digit of x is 7. */
static bool largest_digit_is_seven(uint64_t x)
{
    uint8_t maxd = 0;
    for (;;) {
        uint8_t d = x % 10;
        if (d > maxd) maxd = d;
        x /= 10;
        if (x == 0) break;
    }
    return maxd == 7;
}

/* Count, recursively, the divisors of n whose largest digit is 7.
   cntp points to a seq_t variable holding the current count.   */
static void count_divisors_rec(uint64_t cur, int idx,
                               const uint64_t *pr, const int *ex,
                               int pc, seq_t *cntp)
{
    if (idx == pc) {
        if (largest_digit_is_seven(cur)) (*cntp) += 1;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t val = 1;
    for (int e = 0; e <= ex[idx]; ++e) {
        count_divisors_rec(cur * val, idx + 1, pr, ex, pc, cntp);
        val *= p;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint64_t N = (uint64_t)n;

    /* Factorise N (trial division).  At most 64 distinct primes. */
    uint64_t primes[64];
    int exps[64];
    int pc = 0;
    uint64_t temp = N;
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pc] = p;
            exps[pc]  = e;
            ++pc;
        }
    }
    if (temp > 1) {  /* remaining prime factor */
        primes[pc] = temp;
        exps[pc]  = 1;
        ++pc;
    }

    seq_t result = 0;
    count_divisors_rec(1, 0, primes, exps, pc, &result);
    return result;
}
