/*
 * A081389  Number of non-unitary prime divisors of Catalan numbers, i.e.,
 *          number of those prime factors whose exponent is greater than one.
 * https://oeis.org/A081389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime divisors p of Catalan(n) for which the
 *            exponent of p in Catalan(n) is greater than one. Catalan(n) =
 *            binomial(2n,n)/(n+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Uses Legendre’s formula for the exponent of a prime p in n!:
      v_p(n!) = sum_{k>=1} floor(n/p^k).  */

/*  For a prime p the exponent in Catalan(n) is
        v_p(Catalan(n)) = v_p((2n)!) - 2*v_p(n!) - v_p(n+1)
     So we count those primes for which this value > 1.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static int *prime_list = NULL;     /* primes up to the current sieve limit   */
static size_t prime_count = 0;     /* number of primes stored                */
static uint64_t sieve_limit = 0;   /* highest value for which primes are known */


/* Build a sieve of Eratosthenes up to 'limit' and store primes
   in prime_list.  The sieve grows only once; subsequent calls
   with a larger limit extend it.                                 */
static void build_primes(uint64_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* Allocate a bitmap of booleans (char is 1 byte). */
    size_t sz = (size_t)limit + 1;
    bool *is_prime = (bool *)calloc(sz, sizeof(bool));
    if (!is_prime)
        return;       /* allocation failure – should never happen */

    /* Mark all as prime initially (indices < 2 will be overwritten). */
    for (uint64_t i = 2; i <= limit; ++i)
        is_prime[i] = true;

    /* Eratosthenes sieve. */
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (!is_prime[i])
            continue;
        for (uint64_t j = i * i; j <= limit; j += i)
            is_prime[j] = false;
    }

    /* Count primes for reallocation. */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++cnt;

    int *tmp = (int *)realloc(prime_list, cnt * sizeof(int));
    if (!tmp) {
        free(is_prime);
        return;
    }
    prime_list = tmp;

    /* Copy primes into the list. */
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            prime_list[idx++] = (int)i;

    prime_count = cnt;
    sieve_limit = limit;
    free(is_prime);
}

/* Legendre’s formula: exponent of prime p in n!. */
static uint64_t legendre(uint64_t n, uint64_t p)
{
    uint64_t exp = 0;
    while (n) {
        n /= p;
        exp += n;
    }
    return exp;
}

/*  Main sequence function.  */
seq_t a(idx_t n)
{
    if (n <= 1)            /* offset 1: a(1) = 0 */
        return (seq_t)0;

    uint64_t N = (uint64_t)n;
    uint64_t up_to = 2 * N;

    build_primes(up_to);          /* ensure we have all primes <= 2n */

    seq_t count = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        int p = prime_list[i];
        if ((uint64_t)p > up_to)
            break;

        /* Exponent of p in binomial(2n,n) */
        uint64_t exp_bin = legendre(2 * N, (uint64_t)p)
                      - 2 * legendre(N, (uint64_t)p);

        /* Exponent of p in (n+1) */
        uint64_t m = N + 1;
        uint64_t exp_n1 = 0;
        while (m % (uint64_t)p == 0) {
            ++exp_n1;
            m /= (uint64_t)p;
        }

        uint64_t exp_cat = exp_bin - exp_n1;

        if (exp_cat > 1)
            ++count;
    }

    return count;
}
