/*
 * A074130  Group successively larger prime numbers so that the sum of the
 *          n-th group is a multiple of n. Sequence gives the number of prime
 *          in each group.
 * https://oeis.org/A074130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Iteratively build consecutive groups of primes.
   Let G_n be the n‑th group.  Add successive primes to G_n until
   the group sum S satisfies S ≡ 0 (mod n).  The sequence value
   a(n) is |G_n|, the number of primes in that group.
   The function memoises results so repeated calls are fast. */

#include <stddef.h>
#include <stdint.h>

static uint64_t next_candidate = 2;   /* next odd number tried for primality */
static size_t next_prime_index = 0;   /* index of next unused prime in `primes` */

static uint64_t *primes = NULL;      /* dynamic array of discovered primes    */
static size_t primes_cap  = 0;        /* capacity of the prime array           */
static size_t primes_cnt  = 0;        /* current number of stored primes       */

/* Append a prime to the primes array, reallocating if necessary. */
static void add_prime(uint64_t p)
{
    if (primes_cnt == primes_cap) {
        size_t newcap = primes_cap ? primes_cap * 2 : 64;
        uint64_t *tmp = (uint64_t *)realloc(primes, newcap * sizeof(uint64_t));
        primes = tmp;
        primes_cap = newcap;
    }
    primes[primes_cnt++] = p;
}

/* Simple trial division using previously found primes.  Returns 1 if
   n is prime, 0 otherwise.  n fits in uint64_t, so this is fast enough
   for the required range. */
static int is_prime_trial(uint64_t n)
{
    if (n < 2) return 0;
    if (n % 2 == 0) return n == 2;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > n / p) break;          /* p*p > n */
        if (n % p == 0) return 0;
    }
    return 1;
}

/* Generate primes until the primes array contains at least `target`
   primes.  `target` is usually `primes_cnt + 1000` to keep the array
   reasonably large. */
static void generate_more_primes(size_t target)
{
    while (primes_cnt < target) {
        if (next_candidate == 2) {         /* handle the prime 2 once */
            add_prime(2);
            next_candidate = 3;
            continue;
        }
        if (is_prime_trial(next_candidate))
            add_prime(next_candidate);
        next_candidate += 2;               /* only odd numbers after 2 */
    }
}

/* Cache of a(n) values: static array large enough for n up to 20000.
   The first element is a dummy so that a(1) is at cache[1]. */
static seq_t cache[20001];
static size_t cache_cnt = 1;               /* number of filled entries (1 dummy) */

/* Main sequence function demanded by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* offset is 1; ignore 0 */

    /* Ensure enough cache entries exist up to n. */
    while (cache_cnt <= (size_t)n) {
        size_t group = cache_cnt;          /* current group number (1‑based) */
        unsigned int sum_mod = 0;          /* running sum modulo group */
        unsigned int count   = 0;          /* number of primes in this group */

        /* Accumulate primes until the sum is divisible by group. */
        while (1) {
            if (next_prime_index >= primes_cnt)
                generate_more_primes(primes_cnt + 1000);   /* ensure more primes */

            uint64_t p = primes[next_prime_index++];
            sum_mod = (sum_mod + (unsigned int)(p % group)) % (unsigned int)group;
            ++count;
            if (sum_mod == 0)
                break;                     /* group complete */
        }

        /* Store the computed group size. */
        if (cache_cnt >= sizeof(cache) / sizeof(cache[0]))
            return 0;                     /* safeguard – should never happen */
        cache[cache_cnt++] = (seq_t)count;
    }

    return cache[(size_t)n];
}
