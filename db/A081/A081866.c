/*
 * A081866  a(n) = sigma_9(2n-1).
 * https://oeis.org/A081866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 69, u128 n <= 1000,
 *            big any n
 * Helpers:   sigma_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS entry A081866
 *  a(n) = sigma_9(2n-1)   (sum of the 9th powers of the divisors of 2n-1) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_prime_power.h"

/*--- Prime table -----------------------------------------------------------*/
static int  primes[20000];
static int  prime_cnt = 0;
static bool primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized) return;

    const int limit = 20000;
    bool *composite = (bool*)malloc((limit + 1) * sizeof(bool));
    for (int i = 0; i <= limit; ++i) composite[i] = false;

    for (int i = 2; i <= limit; ++i) {
        if (!composite[i]) {
            primes[prime_cnt++] = i;
            if ((long long)i * i <= limit) {
                for (long long j = (long long)i * i; j <= limit; j += i)
                    composite[(int)j] = true;
            }
        }
    }
    free(composite);
    primes_initialized = true;
}

/*--- Main sequence function ---------------------------------------------*/
seq_t a(idx_t n)
{
    /* m = 2n-1; m fits in idx_t for all practical n */
    idx_t m = (idx_t)2 * n - 1;
    if (m <= 1) return (seq_t)1;  /* sigma_9(1) = 1 */

    init_primes();

    seq_t result = 1;
    idx_t tmp = m;

    for (int i = 0; i < prime_cnt; ++i) {
        int p = primes[i];
        idx_t p2 = (idx_t)p * (idx_t)p;
        if (p2 > tmp) break;

        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            seq_t s = sigma_prime_power(p, e);
            result *= s;
        }
    }

    if (tmp > 1) {            /* remaining prime factor */
        seq_t s = sigma_prime_power((int)tmp, 1);
        result *= s;
    }

    return result;
}
