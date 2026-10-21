/*
 * A073683  Group the primes such that the sum of each group is a prime. Each
 *          group from the second onwards should contain at least 3 primes:
 *          (2, 3), (5, 7, 11), (13, 17, 19, 23, 29), (31, 37, 41), (43, 47,
 *          53, 59, 61), ... This is the sequence of the leading element in
 *          each group.
 * https://oeis.org/A073683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 27, u32 n <= 26, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A073683: first element of the n‑th group of successive primes whose block sum is prime.
    Algorithm:  iterate through the list of primes, greedily extending each block
    until its sum becomes prime.  Store the first prime of each block; this is
    a(n).  The implementation uses incremental prime generation combined with
    a fast primality test (is_prime_big).  The sequence is computed lazily and
    memoised so that subsequent calls for arbitrary indices are efficient.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

static seq_t *prime_list   = NULL;
static size_t prime_count  = 0;
static size_t prime_cap    = 0;

static seq_t *group_start  = NULL;
static size_t group_count  = 0;
static size_t group_cap    = 0;

static size_t prime_index_next = 0;        /* next prime to start a group */

/* ensure we have at least 'need' primes in prime_list */
static void ensure_primes_upto(size_t need)
{
    if (prime_count < need)
    {
        /* grow capacity */
        if (prime_count >= prime_cap)
        {
            size_t new_cap = prime_cap ? prime_cap * 2 : 32;
            prime_list = (seq_t *)realloc(prime_list, new_cap * sizeof(seq_t));
            prime_cap = new_cap;
        }

        seq_t candidate = (prime_count == 0) ? 2 : prime_list[prime_count - 1] + 1;
        if (candidate <= 2)
            candidate = 2;
        else if ((candidate & 1) == 0)
            candidate++;

        while (prime_count < need)
        {
            /* test candidate */
            if (is_prime_big(candidate))
            {
                prime_list[prime_count++] = candidate;
            }
            candidate++;
            if ((candidate & 1) == 0)
                candidate++;
        }
    }
}

/* find the first element of the n-th group (n>=1) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n=0, return 0 */

    /* initialise prime list if empty */
    if (prime_count == 0)
        ensure_primes_upto(1);

    /* generate groups until we have n groups */
    while (group_count < (size_t)n)
    {
        /* ensure prime available for current start */
        ensure_primes_upto(prime_index_next + 1);
        seq_t start_prime = prime_list[prime_index_next];
        idx_t min_len = (group_count == 0) ? 2 : 3;

        idx_t len = 0;
        seq_t sum = 0;
        /* accumulate at least min_len primes */
        while (len < min_len)
        {
            ensure_primes_upto(prime_index_next + len + 1);
            sum += prime_list[prime_index_next + len];
            len++;
        }
        /* extend block until sum is prime */
        while (!is_prime_big(sum))
        {
            len++;
            ensure_primes_upto(prime_index_next + len);
            sum += prime_list[prime_index_next + len - 1];
        }

        /* store start of this block */
        if (group_count + 1 > group_cap)
        {
            size_t new_cap = group_cap ? group_cap * 2 : 32;
            group_start = (seq_t *)realloc(group_start, new_cap * sizeof(seq_t));
            group_cap = new_cap;
        }
        group_start[group_count++] = start_prime;

        /* advance to next group */
        prime_index_next += (size_t)len;
    }

    return group_start[(size_t)n - 1];
}
