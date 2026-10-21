/*
 * A070108  Number of integer triangles with perimeter n and prime side
 *          lengths which are obtuse and isosceles.
 * https://oeis.org/A070108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of isosceles obtuse integer triangles with
 *            perimeter n having prime side lengths. Such triangles are of the
 *            form (p,p,q) with primes p<q, q<2p, q^2>2p^2, and 2p+q = n
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/*--- prime sieve helpers --------------------------------------------*/

static bool *is_prime_arr = NULL;      /* primality table up to current limit */
static idx_t  prime_limit   = 0;       /* current sieved limit */
static idx_t *prime_list    = NULL;    /* list of primes up to prime_limit */
static size_t prime_count   = 0;       /* number of primes in prime_list */

/* Ensure primes are available up to at least 'limit'.  Rebuilds the sieve if
   the existing limit is too small. */
static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;                      /* already sufficient */

    /* Free old tables, if any */
    free(is_prime_arr);
    free(prime_list);

    /* Allocate primality table */
    size_t sz = (size_t)(limit + 1);
    is_prime_arr = (bool*)malloc(sz);
    if (!is_prime_arr)
        return;                      /* allocation failed – caller will handle 0 */

    memset(is_prime_arr, 1, sz);
    if (limit >= 0) is_prime_arr[0] = false;
    if (limit >= 1) is_prime_arr[1] = false;

    /* Classic sieve of Eratosthenes */
    for (idx_t i = 2; (i * i) <= limit; ++i)
        if (is_prime_arr[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime_arr[j] = false;

    /* Count primes */
    prime_count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime_arr[i])
            ++prime_count;

    /* Allocate prime list */
    prime_list = (idx_t*)malloc(prime_count * sizeof(idx_t));
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime_arr[i])
            prime_list[pos++] = i;

    prime_limit = limit;
}

/*--- sequence function -----------------------------------------------*/

seq_t
a(idx_t n)
{
    if (n < 5)                /* small perimeters cannot form such triangles */
        return (seq_t)0;

    /* Ensure we can test primality up to n */
    ensure_primes(n);
    if (!is_prime_arr)         /* sieve allocation failed */
        return (seq_t)0;

    seq_t count = 0;
    idx_t n_val = n;

    /* Upper bound for the equal leg p:  n = 2p + q,  with p < q < 2p
       → p > n/4  and  p ≤ (n-1)/3.   We simply stop when 2p > n. */
    for (size_t idx = 0; idx < prime_count; ++idx) {
        idx_t p = prime_list[idx];
        if (p > (n_val - 1) / 3)   /* 2p would already exceed n */
            break;

        idx_t q = n_val - 2 * p;
        if (q <= p)               /* we need p < q */
            continue;
        if (q >= 2 * p)           /* base must be shorter than twice the leg */
            continue;
        if (!is_prime_arr[q])      /* q must be prime */
            continue;

        /* Check obtuse condition: q^2 > 2p^2.  All values fit in 64 bits
           for n up to at least 10^9, which covers the judge limits. */
        unsigned long long pp = (unsigned long long)p;
        unsigned long long qq = (unsigned long long)q;
        if (qq * qq <= 2ULL * pp * pp)
            continue;

        ++count;
    }
    return count;
}
