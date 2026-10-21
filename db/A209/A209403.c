/*
 * A209403  Convolution of primes with odd primes.
 * https://oeis.org/A209403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} prime(k) * odd_prime(n-k+1)
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 825, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* prime(k)   = k‑th prime (2, 3, 5, 7, …)        */
/* odd_prime(m)=m‑th odd prime (3, 5, 7, 11, …)  */
/*              equals prime(m+1), because prime(1)=2 is even. */

/* The routine builds a list of primes up to 200,000 (enough for the 10,001‑st prime 104,743)
   on the first call and re‑uses the array for any subsequent calls. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t *primes = NULL;   /* primes[0] = 2, primes[1] = 3, …              */
static size_t    primes_cnt = 0;  /* number of primes stored                    */

/* ------------  Sieve of Eratosthenes  ------------------- */
static void init_primes(void)
{
    if (primes != NULL)
        return;                      /* already initialized */

    /* 200,000 comfortably exceeds the 10,001‑st prime (104,743). */
    const idx_t limit   = 200000;
    const size_t cap_init = 20000;   /* initial capacity for primes array          */

    /* Allocate primes array. */
    primes = (uint64_t*)malloc(cap_init * sizeof(uint64_t));
    if (primes == NULL)
        return;                      /* graceful failure – the function will return 0 if this happens */

    primes_cnt = 0;

    /* Allocate a flag array – one byte per number (bool is 1 byte on C/C++). */
    bool *flags = (bool*)calloc(limit + 1, sizeof(bool));
    if (flags == NULL) {
        free(primes);
        primes = NULL;
        return;
    }

    /* Mark all numbers as potential primes. */
    for (idx_t i = 2; i <= limit; ++i)
        flags[i] = true;

    /* Perform the sieve. */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (flags[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                flags[j] = false;
        }
    }

    /* Store the primes into the array, enlarging as needed. */
    size_t cap = cap_init;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!flags[i])
            continue;
        if (primes_cnt == cap) {
            cap *= 2;
            uint64_t *tmp = (uint64_t*)realloc(primes, cap * sizeof(uint64_t));
            if (tmp == NULL)        /* stop if realloc fails – future values may be wrong */
                break;
            primes = tmp;
        }
        primes[primes_cnt++] = (uint64_t)i;
    }

    free(flags);
    /* primes_cnt should now be >= 10001 (there are 10,001 primes up to 104,743). */
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* value for n < 1 is irrelevant to the problem */

    init_primes();      /* ensures the primes array exists */

    seq_t sum = 0;

    /* Compute the convolution.  prime(k) = primes[k-1] (1‑based). */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t pk = (seq_t)primes[k - 1];           /* prime(k)   */
        seq_t q  = (seq_t)primes[(n - k) + 1];     /* odd_prime(n-k+1) = primes[n-k+1] */
        sum += pk * q;
    }

    return sum;
}
