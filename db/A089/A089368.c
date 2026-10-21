/*
 * A089368  Least k such that 2*pi(n) = pi(n+k), where pi(n) = number of
 *          primes up to n. (The number of primes between 1 to n is the same
 *          as the number of primes between n+1 and n+k.)
 * https://oeis.org/A089368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k ≥0 such that π(n + k) = 2·π(n)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (π(x) = number of primes ≤ x)  */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* --- lazy prime‐count sieve ------------------------------------------ */
static unsigned char *is_composite = NULL;       /* 0 = prime, 1 = composite  */
static uint32_t     *pi_table      = NULL;       /* π(x) for x ≤ limit        */
static size_t        sieve_limit   = 0;           /* current maximum stored    */

/* Ensure the sieve contains all primes up to at least LIMIT.  The data
   are rebuilt completely; for the modest limits used here this is fast
   enough.  The function returns when the tables contain at least the
   requested range.  */
static void
ensure_sieve(size_t LIMIT)
{
    if (LIMIT <= sieve_limit)
        return;                       /* already sufficient */

    /* allocate new storage */
    unsigned char *new_is = (unsigned char *)realloc(is_composite, (LIMIT + 1));
    uint32_t     *new_pi = (uint32_t *)realloc(pi_table,     (LIMIT + 1) * sizeof(uint32_t));
    if (!new_is || !new_pi)            /* allocation failure → abort */
        abort();

    is_composite = new_is;
    pi_table     = new_pi;

    /* initialise all entries to 0 (prime) */
    memset(is_composite, 0, LIMIT + 1);

    /* sieve of Eratosthenes */
    size_t prime_count = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!is_composite[i]) { /* i is prime */
            ++prime_count;
            if (i * i <= LIMIT) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    is_composite[j] = 1;
            }
        }
        pi_table[i] = (uint32_t)prime_count;
    }
    pi_table[0] = pi_table[1] = 0;      /* by definition */
    sieve_limit = LIMIT;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)                /* undefined for n < 1 – return 0 */
        return (seq_t)0;

    /* we need prime counts for n and beyond */
    ensure_sieve((size_t)n);

    uint32_t pi_n    = pi_table[(size_t)n];
    uint32_t target  = 2u * pi_n;          /* desired prime count */

    size_t search_at = (size_t)n;
    while (1) {
        if (search_at > sieve_limit)
            ensure_sieve(search_at * 2);   /* grow the sieve if needed */

        uint32_t pi_val = pi_table[search_at];
        if (pi_val == target) {
            size_t k = search_at - (size_t)n;
            return (seq_t)k;              /* cast to seq_t */
        }
        ++search_at;
    }
}
