/*
 * A083897  Number of divisors of n with largest digit <= 2 (base 10).
 * https://oeis.org/A083897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of divisors of n whose decimal digits are all <= 2
 *            (i.e. each divisor has only the digits 0, 1, 2)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Auxiliary helper: test whether all decimal digits of x are ≤ 2. */
#include <stdbool.h>
#include <stdint.h>

static bool allowed_digits(uint64_t x)
{
    while (x > 0) {
        uint64_t d = x % 10;
        if (d > 2) return false;
        x /= 10;
    }
    /* Divisors are always >0, so 0 is not encountered. */
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety: sequence defined for n≥1 */

    /* Factor n into primes using simple trial division. */
    uint64_t primes[16];
    int exps[16];
    int pc = 0;
    uint64_t tmp = (uint64_t)n;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pc] = p;
            exps[pc] = e;
            ++pc;
        }
    }
    if (tmp > 1) {
        primes[pc] = tmp;
        exps[pc] = 1;
        ++pc;
    }

    /* Generate all divisors from the prime factorization. */
    uint64_t divs[256];
    int divc = 1;
    divs[0] = 1;   /* divisor 1 */

    for (int i = 0; i < pc; ++i) {
        uint64_t p = primes[i];
        int e = exps[i];
        int old = divc;
        for (int j = 0; j < old; ++j) {
            uint64_t base = divs[j];
            for (int k = 1; k <= e; ++k) {
                base *= p;
                divs[divc++] = base;
            }
        }
    }

    /* Count the divisors that satisfy the digit condition. */
    seq_t count = 0;
    for (int i = 0; i < divc; ++i) {
        if (allowed_digits(divs[i])) {
            count += 1;
        }
    }

    return count;
}
