/*
 * A126248  p*(p+1)*(p+2) where (p,p+2) are twin primes.
 * https://oeis.org/A126248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p*(p+1)*(p+2) where p and p+2 are the lower and upper
 *            members of the nth twin‑prime pair (OEIS A126248)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#define LIMIT (2000000ULL)          /* upper bound for sieving; 2 million is enough
                                      to cover the first ~12,500 twin primes. */
#define MAX_TWINS 20000             /* space for computed terms */

static seq_t cache[MAX_TWINS];
static idx_t cache_size = 0;
static bool initialized = false;

/* Initialize sieve and precompute all twin‑prime values up to LIMIT. */
static void init(void)
{
    if (initialized)
        return;

    /* Allocate and build a simple sieve. */
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime)
        return; /* allocation failed – unlikely in tests */

    for (idx_t i = 0; i <= LIMIT; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t i = 2; i * i <= LIMIT; ++i)
    {
        if (is_prime[i])
        {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    /* Gather twin primes and compute the required product. */
    for (idx_t p = 3; p + 2 <= LIMIT; p += 2)
    {
        if (is_prime[p] && is_prime[p + 2])
        {
            seq_t prod = (seq_t)p * ((seq_t)p + 1) * ((seq_t)p + 2);
            if (cache_size < MAX_TWINS)
                cache[cache_size++] = prod;
        }
    }

    free(is_prime);
    initialized = true;
}

/* Public function to return the nth term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined – offset is 1 */

    init();

    if (n > cache_size)
        return 0;                 /* not expected for valid inputs within limits */

    return cache[n - 1];
}
