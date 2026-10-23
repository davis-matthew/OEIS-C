/*
 * A036927  Digit sum of 'odd' number equals digit sum of 'sum' and
 *          'juxtaposition' of its prime factors (counted with multiplicity).
 * https://oeis.org/A036927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   build_primes_upto, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th odd number m such that
 *   digit_sum(m) == digit_sum(sum of prime factors (with multiplicity))
 *   and digit_sum(m) == digit_sum(concatenation of prime factors).
 * The function searches odd numbers sequentially and returns the nth one.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/digit_sum.h"

/* static data shared by all calls to a() */
static uint64_t *g_primes = NULL;  /* list of primes up to 200000 */
static size_t   g_prime_cnt = 0;   /* number of primes */
static bool     g_primes_built = false;

/* initialise the prime list once */
static void init_primes(void)
{
    if (g_primes_built)
        return;
    build_primes_upto(200000, &g_primes, &g_prime_cnt);
    g_primes_built = true;
}

/* return the n-th term of the sequence */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined, but harmless */
        return (seq_t)0;

    init_primes();

    idx_t count = 0;            /* terms found so far              */
    uint64_t candidate = 3;     /* first odd > 1                    */
    seq_t result = 0;           /* store the matching term          */

    while (count < n) {
        uint64_t temp = candidate;
        seq_t sum_factors   = 0;
        seq_t sum_conjmp    = 0;   /* digit sum of concatenated primes */
        int   total_exponents = 0;

        /* factorisation by trial division */
        for (size_t i = 0; i < g_prime_cnt && g_primes[i] * g_primes[i] <= temp; ++i) {
            uint64_t p = g_primes[i];
            if (temp % p == 0) {
                int e = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++e;
                }
                sum_factors += (seq_t)p * (seq_t)e;
                sum_conjmp  += digit_sum((seq_t)p) * (seq_t)e;
                total_exponents += e;
            }
        }
        if (temp > 1) {          /* remaining prime factor > sqrt(cand) */
            sum_factors += (seq_t)temp;
            sum_conjmp  += digit_sum((seq_t)temp);
            total_exponents += 1;
        }

        /* reject primes (total_exponents == 1) or numbers with no
           prime factors (candidate == 1) */
        if (total_exponents >= 2) {
            seq_t d1 = digit_sum((seq_t)candidate);
            seq_t d2 = digit_sum(sum_factors);
            seq_t d3 = sum_conjmp;    /* already digit sum of concatenation */
            if (d1 == d2 && d1 == d3) {
                ++count;
                result = (seq_t)candidate;
            }
        }

        candidate += 2;  /* next odd number */
    }

    return result;
}
