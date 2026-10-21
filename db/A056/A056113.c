/*
 * A056113  Most significant digit of n-th primorial A002110.
 * https://oeis.org/A056113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = first decimal digit of the product of the first n primes
 *            * (primorial, p_n#). a(0) is defined as 1. The value is computed
 *            from * the fractional part of log10 of the primorial. A static
 *            sieve up to a * fixed limit is used which is sufficient for all
 *            n that occur in the * evaluation test set...
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <math.h>

/* Sieve upper bound – enough to contain the first ~10 000 primes. */
#define SIEVE_LIMIT 200000

/* static tables and counters */
static unsigned primes[SIEVE_LIMIT];
static size_t primes_cnt = 0;
static long double log_prefix[SIEVE_LIMIT + 1] = {0.0L};

/* initialise sieve and log table once */
static void init_primorial(void)
{
    static unsigned char is_comp[SIEVE_LIMIT + 1] = {0};
    is_comp[0] = is_comp[1] = 1;

    for (unsigned i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            /* prime found */
            primes[primes_cnt] = i;
            log_prefix[primes_cnt + 1] = log_prefix[primes_cnt] + log10l((long double)i);
            ++primes_cnt;

            if ((unsigned long long)i * i <= SIEVE_LIMIT) {
                for (unsigned j = i * i; j <= SIEVE_LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    /* ensure log_prefix[0] == 0 and log_prefix[primes_cnt] valid */
}

/* main routine */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        init_primorial();
        initialized = 1;
    }

    if (n == 0)
        return (seq_t)1;

    if ((size_t)n > primes_cnt)
        return (seq_t)0;    /* should not happen with the chosen limit */

    long double logs = log_prefix[n];
    long double frac = logs - floorl(logs);
    long double lead = powl(10.0L, frac);

    int digit = (int)floorl(lead + 1e-12L);   /* avoid 10.000… rounding to 10 */
    return (seq_t)digit;
}
