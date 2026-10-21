/*
 * A086407  a[1]=1; a[n+1]=a[n]-1 if mod[prime[a[n]],6]=5, else a[n+1]=a[n]+4.
 * https://oeis.org/A086407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*  OEIS A086407
    a(1) = 1
    a(n+1) = a(n)-1 if prime[a(n)] mod 6 == 5 else a(n)+4
    (prime[k] is the k‑th prime, 1‑based) */

static bool primes_ready = false;
static unsigned int *prime_arr = NULL;
static size_t prime_cnt = 0;

/* Pre–compute the first many primes – enough for n up to ~10 000. */
static void init_primes(void)
{
    if (primes_ready) return;

    const size_t LIMIT = 400000;            /* sufficient for the required n */
    unsigned char *sieve = (unsigned char *)malloc(LIMIT + 1);
    if (!sieve) return;                    /* out of memory – let the program crash */

    for (size_t i = 0; i <= LIMIT; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = 0;

    /* Allocate room for the primes; 50000 is comfortably above the needed count */
    prime_arr = (unsigned int *)malloc(50000 * sizeof(unsigned int));
    for (size_t i = 2; i <= LIMIT; ++i)
        if (sieve[i])
            prime_arr[prime_cnt++] = (unsigned int)i;

    free(sieve);
    primes_ready = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, but harmless */

    init_primes();

    seq_t val = 1;                         /* a(1) */
    if (n == 1) return val;

    for (idx_t i = 1; i < n; ++i)
    {
        /* current term is 'val'; need prime[val] (1‑based) */
        size_t idx = (size_t)(val - 1);
        if (idx >= prime_cnt) return 0;  /* safety check – should not happen */

        unsigned int p = prime_arr[idx];

        if (p % 6 == 5)
            val = val - 1;
        else
            val = val + 4;
    }

    return val;
}
