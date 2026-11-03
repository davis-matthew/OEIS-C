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
 * Range:     u8 n <= 8, u16 n <= 27, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   is_prime_u64, mem
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
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
            prime_list = mem_resize(seq_t, prime_list, new_cap);
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
            group_start = mem_resize(seq_t, group_start, new_cap);
            group_cap = new_cap;
        }
        group_start[group_count++] = start_prime;

        /* advance to next group */
        prime_index_next += (size_t)len;
    }

    return group_start[(size_t)n - 1];
}
