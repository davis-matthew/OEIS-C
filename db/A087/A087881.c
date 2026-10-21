/*
 * A087881  a(n) = Prime[Prime[n-1]] + PrimePi[Abs[n - PrimePi[n-1]]].
 * https://oeis.org/A087881
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Prime[Prime[n-1]] + PrimePi[Abs[n - PrimePi[n-1]]]
 * Range:     u8 n <= 17, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses O(1) time per call after an initial sieve up to a fixed upper bound. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Helper: build a prime list large enough for the expected range */
static int *prime_list      = NULL;  /* array of primes (int, 1‑based index in array +1) */
static size_t prime_count   = 0;      /* number of primes stored */
static size_t sieve_limit   = 0;      /* limit up to which sieve was performed */

static void init_primes(void)
{
    if (prime_list != NULL) return;   /* already initialized */

    /* Choose a robust upper bound: 3,000,000 gives > 216,000 primes,
       enough for indices up to ~200k (which covers the 104,729‑th prime). */
    const size_t LIMIT = 3000000;

    unsigned char *sieve = (unsigned char *)calloc(LIMIT + 1, 1);
    if (!sieve) return;  /* allocation failure – caller may abort */

    size_t count = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
    {
        if (!sieve[i])
        {
            ++count;
            if (i * i <= LIMIT)          /* avoid overflow */
                for (size_t j = i * i; j <= LIMIT; j += i)
                    sieve[j] = 1;
        }
    }

    prime_list = (int *)malloc(count * sizeof(int));
    if (!prime_list)          /* allocation failure */
    {
        free(sieve);
        return;
    }

    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!sieve[i]) prime_list[idx++] = (int)i;

    prime_count = count;
    sieve_limit = LIMIT;
    free(sieve);
}

/* Return the k‑th prime (1‑based).  Assumes init_primes() has been called
   and that k <= prime_count. */
static seq_t get_prime_idx(idx_t k)
{
    /* Convert to 0‑based array index.  Since idx_t is unsigned, guard underflow. */
    if (k == 0 || (size_t)k > prime_count) return 0;
    return (seq_t)prime_list[(size_t)k - 1];
}

/* Return π(x) = number of primes ≤ x, using binary search on prime_list. */
static idx_t pi_of_int(idx_t x)
{
    if (x < 2) return 0;

    idx_t lo = 0, hi = (idx_t)prime_count;
    while (lo < hi)
    {
        idx_t mid = (lo + hi) >> 1;
        if ((idx_t)prime_list[(size_t)mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2) return 0;      /* undefined for n < 2, return 0 as placeholder */

    init_primes();           /* ensure primes are available */

    /* Compute prime indices and values. */
    seq_t p_n_minus1   = get_prime_idx(n - 1);            /* p_{n-1} */
    seq_t p_p_n_minus1 = get_prime_idx((idx_t)p_n_minus1); /* p_{p_{n-1}} */

    idx_t pi_n_minus1 = pi_of_int(n - 1);                 /* π(n-1) */
    idx_t diff = (n > pi_n_minus1) ? (n - pi_n_minus1) : (pi_n_minus1 - n);
    idx_t pi_diff = pi_of_int(diff);                      /* π(|n-π(n-1)|) */

    return p_p_n_minus1 + (seq_t)pi_diff;
}
