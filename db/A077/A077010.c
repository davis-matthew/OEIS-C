/*
 * A077010  Jacobi symbol(sigma(2n+1),2n+1).
 * https://oeis.org/A077010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Jacobi( σ(2n+1), 2n+1 ) where σ is the sum‑of‑divisors
 *            function
 * Range:     i8 n <= 54, i16 n <= 99, i32 n <= 99, i64 n <= 99, i128 n <= 99,
 *            big any n
 * Helpers:   jacobi_symbol_ll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This implementation uses a small static sieve of primes up to 50000
   (more than sufficient for sqrt(2n+1) for n <= 100 000) and computes
   σ(m) by prime factorisation.  The Jacobi symbol is evaluated with a
   standard binary algorithm, safe for 64‑bit arguments.  The function
   works for any order of calls without external state.
*/

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for intptr_t, uintptr_t, etc. */
#include "../utils/jacobi_symbol_ll.h"

/* static arrays for the sieve */
#define PRIME_CAP 50050
static int primes[PRIME_CAP];
static int primes_cnt = 0;
static int primes_init = 0;

static void init_primes(void)
{
    if (primes_init)
        return;

    int N = 50000;
    char mark[N + 1];
    for (int i = 2; i <= N; ++i)
        mark[i] = 1;

    for (int i = 2; i * i <= N; ++i) {
        if (mark[i]) {
            for (int j = i * i; j <= N; j += i)
                mark[j] = 0;
        }
    }

    for (int i = 2; i <= N; ++i)
        if (mark[i])
            primes[primes_cnt++] = i;

    primes_init = 1;
}

/* compute σ(m) for an odd m (m >= 1) */
static seq_t sigma_odd(idx_t m)
{
    seq_t result = 1;
    idx_t temp = m;

    for (int i = 0; i < primes_cnt; ++i) {
        int p = primes[i];
        idx_t pp = (idx_t)p;
        if (pp * pp > temp)
            break;
        if (temp % pp == 0) {
            seq_t term = 1;
            idx_t power = 1;
            while (temp % pp == 0) {
                temp /= pp;
                power *= pp;
                term += power;
            }
            result *= term;
        }
    }

    if (temp > 1)          /* remaining prime factor */
        result *= (1 + temp);

    return result;
}

/* sequence entry */
seq_t a(idx_t n)
{
    init_primes();

    idx_t m = 2 * n + 1;          /* odd index */
    seq_t sigma = sigma_odd(m);   /* σ(m) */
    int val = jacobi_symbol_ll((long long)sigma, (long long)m);
    return (seq_t)val;
}
