/*
 * A051634  Strong primes: prime(k) > (prime(k-1) + prime(k+1))/2.
 * https://oeis.org/A051634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) is the n‑th prime p_k such that * 2·p_k > p_{k-1} +
 *            p_{k+1}. (OEIS A051634) * The function generates all primes up
 *            to a fixed limit * (400 000), then scans them for the desired
 *            property. * It stops when the n‑th “strong prime’’ is found. *
 *            The limit is large enough to contain ≥ 10 000 such p...
 * Range:     u8 n <= 23, u16 n <= 3097, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define PRIME_LIMIT 400000     /* safe upper bound for 20000 primes   */
#define MAX_PRIMES 40000       /* maximum number of primes stored  */

/* static storage for primes */
static seq_t *primes = NULL;    /* array of primes */
static int prime_cnt = 0;       /* number of primes generated */
static bool initialized = false;

/* one–time initialization: sieve and collect primes */
static void init_primes(void)
{
    if (initialized) return;

    /* allocate boolean array for sieve */
    unsigned char *is_composite = (unsigned char*)
        malloc(PRIME_LIMIT + 1);
    if (!is_composite) exit(1);          /* abort if allocation fails */

    for (int i = 2; i <= PRIME_LIMIT; ++i)
        is_composite[i] = 0;

    for (int i = 2; (long long)i * i <= PRIME_LIMIT; ++i)
        if (!is_composite[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                is_composite[j] = 1;

    /* allocate primes array */
    primes = (seq_t *)malloc(MAX_PRIMES * sizeof(seq_t));
    if (!primes) exit(1);

    prime_cnt = 0;
    for (int i = 2; i <= PRIME_LIMIT && prime_cnt < MAX_PRIMES; ++i)
        if (!is_composite[i])
            primes[prime_cnt++] = (seq_t)i;

    free(is_composite);
    initialized = true;
}

/* compute the n-th strong prime */
seq_t a(idx_t n)
{
    /* ensure primes are ready */
    init_primes();

    int strong = 0;          /* count of strong primes seen */
    for (int k = 1; k + 1 < prime_cnt; ++k) {
        /* 2·prime_k > prime_{k-1} + prime_{k+1}  */
        if ((seq_t)2 * primes[k] > primes[k - 1] + primes[k + 1]) {
            ++strong;
            if ((idx_t)strong == n)
                return primes[k];
        }
    }

    /* If we reach here, the limit was too small.  This should not
       happen for the intended input range (n ≤ 10 000).  */
    return 0;
}
