/*
 * A178105  Let B_n be the set of divisors 2 <= d <= n/2 of
 *          binomial(n-d-1,d-1) such that gcd(n,d)>1. The sequence lists the
 *          minimal d of B_n, or a(n)=0 if B_n is empty.
 * https://oeis.org/A178105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function implements A178105:  for each n >= 1 we look for the
 * smallest divisor d with 2 <= d <= n/2 such that
 *   - gcd(n,d) > 1
 *   - d divides the binomial coefficient C(n-d-1 , d-1)
 * If no such d exists the function returns 0.
 *
 * The algorithm factors each candidate d and compares the exponents of the
 * primes in d with those in the binomial coefficient (using Legendre's
 * formula).  The smallest qualifying d is returned.  The routine works for
 * any seq_t and idx_t types supplied by the build. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>   /* for uint64_t */

/* -------------------------------------------------------------------- */
/* Prime factorisation helper: smallest prime factor table             */
/* -------------------------------------------------------------------- */

static idx_t *spf = NULL;      /* smallest prime factor table      */
static idx_t spf_size = 0;     /* size of the table (last index)   */

/* Ensure the sieve table is computed up to at least 'limit'. */
static void
ensure_spf(idx_t limit)
{
    if (limit <= spf_size)
        return;                       /* already available */

    /* enlarge the table */
    idx_t new_size = limit;
    idx_t *new_tbl = (idx_t *)malloc((new_size + 1) * sizeof(idx_t));
    if (!new_tbl)
        abort();                       /* out of memory */

    /* initialise */
    for (idx_t i = 0; i <= new_size; ++i)
        new_tbl[i] = 0;
    new_tbl[0] = 0;
    new_tbl[1] = 1;

    /* sieve */
    for (idx_t i = 2; i <= new_size; ++i) {
        if (new_tbl[i] == 0) {                /* i is prime */
            new_tbl[i] = i;
            if (i <= new_size / i) {          /* avoid overflow */
                for (idx_t j = i * i; j <= new_size; j += i) {
                    if (new_tbl[j] == 0)
                        new_tbl[j] = i;
                }
            }
        }
    }

    /* replace old table */
    if (spf)
        free(spf);
    spf = new_tbl;
    spf_size = new_size;
}

/* -------------------------------------------------------------------- */
/* Exponent of prime p in n!   (Legendre's formula)                     */
/* -------------------------------------------------------------------- */

static idx_t
exp_in_factorial(idx_t n, idx_t p)
{
    idx_t e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

/* -------------------------------------------------------------------- */
/* Main sequence function                                               */
/* -------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 2)                     /* n = 1 gives no candidates */
        return (seq_t)0;

    /* We only need to factor divisors up to n/2 */
    ensure_spf(n / 2);

    for (idx_t d = 2; d <= n / 2; ++d) {
        /* gcd(n,d) must be > 1 */
        idx_t x = n, y = d;
        while (y) {                 /* Euclidean algorithm */
            idx_t t = x % y;
            x = y;
            y = t;
        }
        if (x == 1)
            continue;               /* coprime, skip */

        idx_t N = n - d - 1;        /* upper argument of binomial   */
        idx_t K = d - 1;            /* lower argument of binomial   */

        /* Check prime exponents of d in the binomial coefficient. */
        idx_t tmp = d;
        bool ok = true;

        while (tmp > 1 && ok) {
            idx_t p = spf[tmp];
            idx_t cnt_d = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt_d;
            }

            idx_t exp_binom = exp_in_factorial(N, p)
                            - exp_in_factorial(K, p)
                            - exp_in_factorial(N - K, p);

            if (exp_binom < cnt_d)
                ok = false;
        }

        if (ok)
            return (seq_t)d;        /* smallest qualifying divisor found */
    }

    return (seq_t)0;                /* no divisor satisfies the condition */
}
