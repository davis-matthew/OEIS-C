/*
 * A105470  a(n)=1 if there is number of the form 6k+3 with prime(n) <= 6k+3
 *          <= prime(n+1), otherwise 0.
 * https://oeis.org/A105470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if there exists m ≡ 3 (mod 6) with prime(n) ≤ m ≤
 *            prime(n+1), otherwise 0
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* --------------------------------------------------------------------
 * Prime number cache.  The cache stores the first N primes in a
 * dynamically growing array.  Only the smallest number of primes enough
 * for the current call to a(idx_t n) is generated.
 * -------------------------------------------------------------------- */
static uint64_t *primes = NULL;   /* array of primes, 1‑based in the OEIS sense */
static size_t   prime_count = 0;   /* number of stored primes          */
static size_t   prime_cap   = 0;   /* current malloced capacity        */

/* Grow the prime cache when the capacity is exhausted.  */
static void prime_extend(size_t new_cap)
{
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    primes     = tmp;
    prime_cap  = new_cap;
}

/* Ensure that at least `required` primes (1‑based) are stored in the cache. */
static void prime_ensure(size_t required)
{
    if (prime_count >= required)
        return;                     /* already have enough primes */

    if (!primes) {                  /* first allocation */
        prime_cap = 16;
        primes = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_count = 0;
    }

    if (prime_count == 0) {         /* prime(1) = 2 */
        primes[prime_count++] = 2;
    }

    /* Start testing odd numbers.  After 2 the next candidate is 3. */
    uint64_t candidate = (prime_count == 1) ? 3 :
                         primes[prime_count - 1] + 2;

    while (prime_count < required) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = primes[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_count == prime_cap)
                prime_extend(prime_cap * 2);
            primes[prime_count++] = candidate;
        }
        candidate += 2;             /* next odd */
    }
}

/* --------------------------------------------------------------------
 * OEIS sequence A105470
 * -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* undefined for indices below offset */

    /* We need prime(n) and prime(n+1).  primes array is 0‑based, so we
       request `n + 1` primes (primes[0]..primes[n])  */
    prime_ensure((size_t)n + 1);

    uint64_t p = primes[(size_t)n - 1];   /* prime(n)   */
    uint64_t q = primes[(size_t)n];       /* prime(n+1) */

    /* Smallest integer ≥ p that is congruent to 3 mod 6.  */
    uint64_t mod = p % 6;
    uint64_t offset = (mod <= 3) ? (3 - mod) : (9 - mod);
    uint64_t m = p + offset;

    return (seq_t)(m <= q);
}
