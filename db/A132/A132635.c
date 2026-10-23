/*
 * A132635  Number of primes, 0's, and 1's in [0, n^2).
 * https://oeis.org/A132635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = pi(n^2) + 2
 * Range:     u8 n <= 40, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Global cache: keeps the largest bound that has already been
   sieved together with the prime list up to that bound. */
static idx_t        __max_limit = 0;       /* largest bound already sieved   */
static uint32_t *   __prime_list = NULL;   /* primes <= __max_limit          */
static size_t      __prime_count = 0;      /* number of stored primes        */

/* Helper macros for bit‑set operations on the composite array. */
#define BIT_GET(arr,i)   (((arr)[(i)>>3] >> ((i)&7)) & 1U)
#define BIT_SET(arr,i)   ((arr)[(i)>>3] |= (1U << ((i)&7)))

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Square bound */
    idx_t sq = (idx_t)n * (idx_t)n;

    /* Sieve up to sq if necessary */
    if (sq > __max_limit) {
        size_t bits_len = (sq >> 3) + 1;
        unsigned char *composite = (unsigned char *)calloc(bits_len, 1);
        if (!composite) return (seq_t)0;   /* out of memory – abort */

        /* Classic Eratosthenes sieve */
        for (idx_t p = 2; p * p <= sq; ++p) {
            if (!BIT_GET(composite, p)) {
                for (idx_t q = p * p; q <= sq; q += p)
                    BIT_SET(composite, q);
            }
        }

        /* Count primes to determine list size */
        size_t cnt = 0;
        for (idx_t i = 2; i <= sq; ++i)
            if (!BIT_GET(composite, i)) ++cnt;

        /* Allocate (and enlarge) the prime list */
        uint32_t *new_list = (uint32_t *)realloc(__prime_list, cnt * sizeof(uint32_t));
        if (!new_list) {
            free(composite);
            return (seq_t)0;   /* out of memory – abort */
        }
        __prime_list = new_list;

        /* Fill the list with primes */
        size_t idx = 0;
        for (idx_t i = 2; i <= sq; ++i)
            if (!BIT_GET(composite, i))
                __prime_list[idx++] = (uint32_t)i;

        __prime_count = cnt;
        __max_limit   = sq;
        free(composite);
    }

    /* Binary search to find π(sq) = number of primes ≤ sq */
    size_t lo = 0, hi = __prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if ((__prime_list[mid] <= (uint32_t)sq))
            lo = mid + 1;
        else
            hi = mid;
    }

    seq_t pi_val = (seq_t)lo;
    return pi_val + (seq_t)2;
}
