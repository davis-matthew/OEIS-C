/*
 * A102539  Square array T(n,k) read by antidiagonals: T(n,k) =
 *          Product_{1<=i<=j<=k} (n+i+j-1)/(i+j-1).
 * https://oeis.org/A102539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the antidiagonal read of the square array * T(n,k) =
 *            ∏_{1 ≤ i ≤ j ≤ k} (n + i + j - 1)/(i + j - 1) * The function
 *            maps the index to (row, col) and evaluates the product * using
 *            prime factorisation, so the integer operations never overflow *
 *            for 64‑bit arithmetic and unlimited precision fo...
 * Range:     u8 n <= 17, u16 n <= 40, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* ---------- initialise prime list up to 5000 once ---------- */
    static bool primes_inited = false;
    static idx_t primes[2000];
    static size_t prime_cnt = 0;
    if (!primes_inited) {
        const size_t LIM = 5000;
        bool sieve[LIM + 1];
        memset(sieve, true, sizeof(sieve));
        sieve[0] = sieve[1] = false;
        for (size_t p = 2; p * p <= LIM; ++p)
            if (sieve[p])
                for (size_t m = p * p; m <= LIM; m += p)
                    sieve[m] = false;
        for (size_t v = 2; v <= LIM; ++v)
            if (sieve[v]) primes[prime_cnt++] = (idx_t)v;
        primes_inited = true;
    }

    /* ---------- map linear index to (row, col) of T ---------- */
    idx_t idx = n;                   /* indices are 1‑based */
    idx_t s = 2;
    while ((s - 1) * s / 2 < idx) ++s;          /* find minimal s with t(s-1) >= idx */

    idx_t t_prev = 0;
    if (s >= 3) t_prev = (s - 2) * (s - 1) / 2; /* cumulative count before this s */
    idx_t pos  = idx - t_prev - 1;              /* 0‑based position within this anti‑diagonal */
    idx_t row  = s - 1 - pos;                   /* row number n */
    idx_t col  = s - row;                       /* column number k */

    /* ---------- compute T(row, col) by prime factorisation ---------- */
    int32_t exp[2000] = {0};                    /* exponent of each prime */
    for (idx_t i = 1; i <= col; ++i) {
        for (idx_t j = i; j <= col; ++j) {
            idx_t num = row + i + j - 1;
            idx_t den = i   + j - 1;

            /* factorise numerator (add exponents) */
            idx_t temp = num;
            for (size_t pi = 0; pi < prime_cnt && primes[pi] * primes[pi] <= temp; ++pi) {
                if (temp % primes[pi] == 0) {
                    int e = 0;
                    while (temp % primes[pi] == 0) {
                        temp /= primes[pi];
                        ++e;
                    }
                    exp[pi] += e;
                }
            }
            if (temp > 1) {                     /* residual prime factor */
                for (size_t pi = 0; pi < prime_cnt && primes[pi] <= temp; ++pi)
                    if (primes[pi] == temp) { exp[pi] += 1; break; }
            }

            /* factorise denominator (subtract exponents) */
            temp = den;
            for (size_t pi = 0; pi < prime_cnt && primes[pi] * primes[pi] <= temp; ++pi) {
                if (temp % primes[pi] == 0) {
                    int e = 0;
                    while (temp % primes[pi] == 0) {
                        temp /= primes[pi];
                        ++e;
                    }
                    exp[pi] -= e;
                }
            }
            if (temp > 1) {
                for (size_t pi = 0; pi < prime_cnt && primes[pi] <= temp; ++pi)
                    if (primes[pi] == temp) { exp[pi] -= 1; break; }
            }
        }
    }

    /* ---------- assemble the integer result ---------- */
    seq_t result = 1;
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        for (int32_t e = 0; e < exp[pi]; ++e)
            result *= primes[pi];
    }
    return result;
}
