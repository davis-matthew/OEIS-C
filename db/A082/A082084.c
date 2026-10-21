/*
 * A082084  a(n) = A029908(n!) = A029908(A000142(n)); fixed points of iterated
 *          A001414 function if started at factorials as initial values.
 * https://oeis.org/A082084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A082084:
   a(n) = the fixed point of iterated f(x) = sum of prime factors (with multiplicity)
   when starting from n! (the factorial of n).  The sequence is 1‑based. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"

/* sum of prime factors with multiplicity of a non‑negative number x
   (x is assumed to fit in idx_t when cast) */
static seq_t sum_prime_factors(seq_t x)
{
    if (x <= 1) return 0;
    seq_t sum = 0;
    idx_t tmp = (idx_t)x;            /* actual value fits in idx_t for all relevant use */
    for (idx_t p = 2; p * p <= tmp; ++p) {
        while (tmp % p == 0) {
            sum += p;
            tmp /= p;
        }
    }
    if (tmp > 1) sum += tmp;          /* remaining prime factor */
    return sum;
}

/* compute a(n) for n >= 1
   static cache of primes up to the largest n requested so far */
seq_t a(idx_t n)
{
    /* --------------------------------------------------------------
       1.  Prime table: keep it up‑to‑date for the maximum n seen.
       -------------------------------------------------------------- */
    static idx_t *prime_list = NULL;
    static size_t prime_cnt = 0;
    static size_t prime_cap = 0;       /* current capacity of prime_list (max index) */

    if (n > prime_cap) {               /* need more primes */
        if (prime_list != NULL) {
            free(prime_list);
            prime_list = NULL;
        }
        build_primes(n, &prime_list, &prime_cnt);
        prime_cap = n;
    }

    /* --------------------------------------------------------------
       2.  Compute the sum of prime factors of n!:
           sum = Σ_{p <= n} p * ⌊ n / p^k ⌋ ,  summed over all k ≥ 1.
       -------------------------------------------------------------- */
    seq_t sum = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p > n) break;
        idx_t exp = 0;
        idx_t pp = p;
        while (pp <= n) {
            exp += n / pp;
            if (pp > n / p) break;     /* prevent overflow of pp */
            pp *= p;
        }
        sum += (seq_t)exp * p;
    }

    /* --------------------------------------------------------------
       3.  Iterate f(x) = sum_prime_factors(x) until a fixed point is
           reached.  The result is always a prime (for n > 1).
       -------------------------------------------------------------- */
    seq_t x = sum;
    while (true) {
        seq_t next = sum_prime_factors(x);
        if (next == x) break;
        x = next;
    }
    return x;
}
