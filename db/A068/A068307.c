/*
 * A068307  From Goldbach problem: number of decompositions of n into a sum of
 *          three primes.
 * https://oeis.org/A068307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of unordered triples of primes (p≤q≤r) with p+q+r
 *            = n
 * Range:     u8 n <= 376, u16 n <= 8445, u32 n <= 8445, u64 n <= 8445,
 *            u128 n <= 8445, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by a simple sieve + enumeration. */

#include <stdlib.h>
#include <stddef.h>

/* Prime flag array: 1 if prime, 0 otherwise */
static unsigned char *prime_flag = NULL;
static size_t       sieve_limit = 0;

/* List of primes */
static int   *prime_list = NULL;
static size_t  prime_cnt = 0;

/* Build a sieve of Eratosthenes up to nmax and a prime list. */
static void build_sieve(idx_t nmax)
{
    if (nmax <= sieve_limit) return;

    free(prime_flag);
    prime_flag = (unsigned char*)calloc((size_t)nmax + 1, 1);
    if (!prime_flag) exit(1);

    /* 0 and 1 are not prime */
    prime_flag[0] = 0;
    prime_flag[1] = 0;

    /* Mark all >1 as potential prime */
    if (nmax >= 2)
        memset(prime_flag + 2, 1, (size_t)nmax - 1);

    for (idx_t p = 2; p * p <= nmax; ++p)
        if (prime_flag[p])
            for (idx_t q = p * p; q <= nmax; q += p)
                prime_flag[q] = 0;

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= nmax; ++i)
        if (prime_flag[i]) ++cnt;

    prime_list = (int*)realloc(prime_list, cnt * sizeof(int));
    if (!prime_list) exit(1);

    size_t k = 0;
    for (idx_t i = 2; i <= nmax; ++i)
        if (prime_flag[i]) prime_list[k++] = (int)i;

    prime_cnt   = cnt;
    sieve_limit = (size_t)nmax;
}

seq_t a(idx_t n)
{
    /* For n < 6 the value is 0 by definition. */
    if (n < 6) return 0;

    build_sieve(n);

    seq_t total = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        int p = prime_list[i];
        if ((idx_t)p > n) break;

        for (size_t j = i; j < prime_cnt; ++j) {
            int q = prime_list[j];
            idx_t sum2 = (idx_t)p + (idx_t)q;
            if (sum2 > n - 2) break;                /* need r ≥ 2 */

            idx_t r = n - sum2;
            if (r < (idx_t)q) break;                /* keep r ≥ q to avoid duplicates */

            if (prime_flag[(size_t)r]) ++total;
        }
    }

    return total;
}
