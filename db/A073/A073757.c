/*
 * A073757  a(n) = d(n) + phi(n) - 1.
 * https://oeis.org/A073757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = d(n) + phi(n) - 1
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  d(n)  = number of divisors of n
 *  phi(n) = Euler's totient of n
 *  n ≥ 1
 */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state: arrays for phi and divisor count */
    static uint64_t *phi = NULL;          /* phi[i]  = φ(i)   */
    static uint64_t *divcnt = NULL;       /* divcnt[i] = d(i) */
    static uint64_t max_pre = 0;          /* largest n computed */

    if (n == 0) {
        return (seq_t)0;                  /* undefined for 0, safe guard */
    }

    if ((uint64_t)n > max_pre) {
        uint64_t new_n = (uint64_t)n;
        /* allocate space for all numbers up to new_n (1‑based) */
        phi    = (uint64_t*)realloc(phi,    (new_n + 1) * sizeof(uint64_t));
        divcnt = (uint64_t*)realloc(divcnt, (new_n + 1) * sizeof(uint64_t));

        /* initialise phi and divisor count arrays */
        for (uint64_t i = 1; i <= new_n; ++i) {
            phi[i]    = i;   /* initial value for totient sieve */
            divcnt[i] = 0;   /* will be incremented by divisor loops */
        }

        /* Euler's totient sieve */
        for (uint64_t i = 2; i <= new_n; ++i) {
            if (phi[i] == i) {                 /* i is prime */
                for (uint64_t j = i; j <= new_n; j += i) {
                    phi[j] -= phi[j] / i;
                }
            }
        }

        /* divisor count by harmonic sieve */
        for (uint64_t i = 1; i <= new_n; ++i) {
            for (uint64_t j = i; j <= new_n; j += i) {
                divcnt[j] += 1;
            }
        }

        max_pre = new_n;
    }

    return (seq_t)divcnt[n] + (seq_t)phi[n] - 1;
}
