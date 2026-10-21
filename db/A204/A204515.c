/*
 * A204515  a(n) = (2*n)! * (2*n+1)! / ((n+1)^2 * n!^3).
 * https://oeis.org/A204515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n)! * (2*n+1)! / ((n+1)^2 * n!^3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   exp_number, pow_seq_fixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the prime‑factorisation of the expression.
   It is fully generic: it works for the 8/16/32/64 bit and arbitrary
   precision builds.  For each prime p ≤ 2*n+1 the exponent e in a(n) is
    
          e = v_p((2n)!) + v_p((2n+1)!) - 3*v_p(n!) - 2*v_p(n+1).

   The result is built by multiplying p^e for each prime.           */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/exp_number.h"
#include "../utils/pow_seq_fixed.h"

/* --- Helper functions ---------------------------------------------------- */

/* v_p(m!) : exponent of prime p in m! */
static idx_t exp_factorial(idx_t m, idx_t p)
{
    idx_t sum = 0;
    while (m)
    {
        m /= p;
        sum += m;
    }
    return sum;
}

/* --- Prime table -------------------------------------------------------- */

#define PRIME_TABLE_CAP 20005            /* enough for 2*10000+1 */

static idx_t prime_table[PRIME_TABLE_CAP];
static size_t prime_cnt = 0;            /* number of primes stored       */
static idx_t prime_limit = 0;           /* largest number sieved         */

/* Build (or extend) a prime table up to 'limit'.  The table is stored
   in the static arrays above.  If the table already covers the range
   this function returns immediately. */
static void ensure_primes(idx_t limit)
{
    if (prime_limit >= limit)
        return;

    /* start from max(2, prime_limit+1) */
    idx_t start = (prime_limit >= 2) ? prime_limit + 1 : 2;

    for (idx_t n = start; n <= limit; ++n)
    {
        bool is_prime = true;
        idx_t sq = (idx_t)(n * n);
        if (sq > limit)
            sq = limit;

        for (size_t i = 0; i < prime_cnt && prime_table[i] <= sq; ++i)
        {
            if (n % prime_table[i] == 0)
            {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
        {
            prime_table[prime_cnt++] = n;
        }
    }
    prime_limit = limit;
}

/* --- Sequence function --------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Base case: a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Ensure we have primes up to 2*n+1 */
    ensure_primes(2 * n + 1);

    seq_t result = 1;
    idx_t two_n = 2 * n;
    idx_t two_n_plus_one = 2 * n + 1;
    idx_t n_plus_one = n + 1;

    /* Iterate over all primes ≤ 2*n+1 */
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        idx_t p = prime_table[i];
        if (p > two_n_plus_one)
            break;

        /* exponent e of prime p in a(n) */
        idx_t e = exp_factorial(two_n, p)
                + exp_factorial(two_n_plus_one, p)
                - 3 * exp_factorial(n, p)
                - 2 * exp_number(n_plus_one, p);

        if (e == 0)
            continue;

        /* multiply result by p^e */
        seq_t factor = pow_seq_fixed((seq_t)p, e);
        result *= factor;
    }

    return result;
}
