/*
 * A060303  Number of primes below n^2 does not exceed n times the number of
 *          primes below n.
 * https://oeis.org/A060303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 838, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n*π(n) - π(n²)
 * where π(x) is the number of primes ≤ x.
 * The implementation uses an odd‑only bit sieve and keeps the
 * sieve and prime list statically, extending them only when a larger
 * `n²` is requested.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Assume seq_t and idx_t are defined by the build system.  */

static uint8_t *sieve_bits  = NULL;   /* bit array for odd numbers only */
static uint32_t *prime_list = NULL;   /* list of primes up to current sieve_limit */
static size_t    prime_cnt  = 0;
static idx_t    sieve_limit = 0;      /* maximum n² that has been sieved */

/* Macros for odd number sieve.  Number represented by bit index i is 2*i+3. */
#define IS_COMP_ODD(i)   (sieve_bits[(i)>>3] & (1U << ((i)&7)))
#define SET_COMP_ODD(i)  (sieve_bits[(i)>>3] |= (1U << ((i)&7)))

/* Build a sieve up to ‘limit’ (inclusive).  Arrays are reallocated on every build. */
static void build_sieve(idx_t limit)
{
    if (limit < 2) {
        /* Nothing to do – no primes >1.  */
        if (sieve_bits) free(sieve_bits);
        if (prime_list) free(prime_list);
        sieve_bits = NULL;
        prime_list = NULL;
        prime_cnt = 0;
        sieve_limit = 0;
        return;
    }

    /* Free old data, if any. */
    if (sieve_bits) free(sieve_bits);
    if (prime_list) free(prime_list);

    /* Size of bit array: store only odd numbers up to limit. */
    idx_t odd_cnt = (limit >= 3) ? ((limit - 3) / 2 + 1) : 0;
    size_t bits_size = (odd_cnt >> 3) + 1;
    sieve_bits = (uint8_t *)malloc(bits_size);
    if (!sieve_bits) return;                 /* allocation failed – leave empty */
    memset(sieve_bits, 0, bits_size);

    /* 2 is prime; skip odd sieve for it. */
    prime_cnt = 1;                 /* prime 2 */
    /* Estimate upper bound for prime list: π(limit) < limit / log(limit) + 3 */
    double approx = (double)limit / log((double)limit) + 3.0;
    size_t prealloc = (size_t)approx;
    prime_list = (uint32_t *)malloc(prealloc * sizeof(uint32_t));
    if (!prime_list) return;                     /* allocation failed */
    prime_list[0] = 2;
    size_t pos = 1;

    /* Find all primes using a classic efficient odd only sieve. */
    idx_t sqrt_limit = (idx_t)floor(sqrt((double)limit) + 0.5);
    for (idx_t i = 0; i < odd_cnt; ++i) {
        if (IS_COMP_ODD(i)) continue;           /* composite */
        idx_t p = 2 * i + 3;                    /* actual prime */

        /* Store prime */
        if (pos == prealloc) {                  /* expand array if needed */
            prealloc = prealloc * 2 + 16;
            uint32_t *tmp = (uint32_t *)realloc(prime_list, prealloc * sizeof(uint32_t));
            if (!tmp) return;
            prime_list = tmp;
        }
        prime_list[pos++] = (uint32_t)p;

        if (p > sqrt_limit) continue;           /* no further marking needed */

        /* Mark odd multiples of p: start at p*p (which is odd). */
        idx_t start = p * p;
        if (start > limit) continue;
        /* Convert start to index i_start: number 2*i+3 = start → i = (start-3)/2 */
        idx_t j = (start - 3) / 2;
        for (; j < odd_cnt; j += p) {
            SET_COMP_ODD(j);
        }
    }

    prime_cnt = pos;
    sieve_limit = limit;
}

/* Ensure that the sieve covers at least ‘limit’. */
static void ensure_limit(idx_t limit)
{
    if (limit <= sieve_limit) return;
    build_sieve(limit);
}

/* Primitive prime counting function π(x) using binary search on prime_list. */
static seq_t prime_pi(idx_t x)
{
    if (x < 2) return 0;
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if ((idx_t)prime_list[mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (seq_t)lo;
}

/* Sequence function a(n). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Compute n² safely. */
    unsigned __int128 sq128 = (unsigned __int128)n * n;
    idx_t sq = (sq128 > (unsigned __int128)UINT64_MAX) ? UINT64_MAX : (idx_t)sq128;

    /* Extend sieve if necessary. */
    ensure_limit(sq);

    seq_t pi_n  = prime_pi(n);
    seq_t pi_sq = prime_pi(sq);

    return (seq_t)n * pi_n - pi_sq;
}
