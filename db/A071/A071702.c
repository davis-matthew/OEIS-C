/*
 * A071702  Number of twin prime pairs <= n of form (4*k+3,4*(k+1)+1), k>=0.
 * https://oeis.org/A071702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k | 0≤k≤n, (4*k+3) and (4*(k+1)+1) are prime}
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   A071702: Number of twin prime pairs ≤ n of the form (4*k+3, 4*(k+1)+1)  */
/*   (i.e. count the twin pairs whose smaller member is ≤ n).                     */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Cached sieve and twin‑prime table                                      */
/* --------------------------------------------------------------------- */
static idx_t  sieve_limit = 0;          /* largest number currently sieved   */
static bool *prime_flags = NULL;        /* prime_flags[i] == true  ⇔  i is prime   */
static idx_t *twin_list   = NULL;        /* smaller prime of each twin pair   */
static size_t twin_count   = 0;           /* number of twin pairs in twin_list */

/* --------------------------------------------------------------------- */
/*  Build or extend a simple Eratosthenes sieve up to 'limit' (inclusive). */
/* --------------------------------------------------------------------- */
static void rebuild_sieve(idx_t limit)
{
    /* Reallocate the prime flag array. */
    bool *new_flags = (bool*)realloc(prime_flags, (size_t)limit + 1);
    if (!new_flags) exit(EXIT_FAILURE);
    prime_flags = new_flags;
    sieve_limit = limit;

    /* Initialise all entries as prime. */
    for (idx_t i = 0; i <= limit; ++i) prime_flags[i] = true;
    if (limit >= 0) { prime_flags[0] = false; prime_flags[1] = false; }

    /* Eratosthenes sieve.  Use idx_t for indices to avoid signedness issues. */
    for (idx_t i = 2; (idx_t)((i * i) <= limit); ++i)
    {
        if (prime_flags[i])
        {
            for (idx_t j = i * i; j <= limit; j += i) prime_flags[j] = false;
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Build the list of twin primes (smaller member ≤ sieve_limit-2).       */
/* --------------------------------------------------------------------- */
static void rebuild_twin_list(void)
{
    /* First pass: count how many twin primes exist. */
    size_t cnt = 0;
    for (idx_t p = 2; p + 2 <= sieve_limit; ++p)
    {
        if (prime_flags[p] && ((p & 3) == 3) && prime_flags[p + 2]) ++cnt;
    }

    /* Resize the twin list array. */
    idx_t *new_list = (idx_t*)realloc(twin_list, cnt * sizeof(idx_t));
    if (!new_list && cnt > 0) exit(EXIT_FAILURE);
    twin_list = new_list;
    twin_count = cnt;

    /* Second pass: store the twin primes. */
    size_t pos = 0;
    for (idx_t p = 2; p + 2 <= sieve_limit; ++p)
    {
        if (prime_flags[p] && ((p & 3) == 3) && prime_flags[p + 2])
        {
            twin_list[pos++] = p;
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Binary‑search count of twin primes with smaller member ≤ n.           */
/* --------------------------------------------------------------------- */
static seq_t count_twin_primes(idx_t n)
{
    size_t lo = 0, hi = twin_count;
    while (lo < hi)
    {
        size_t mid = lo + (hi - lo) / 2;
        if (twin_list[mid] <= n) lo = mid + 1;
        else                      hi = mid;
    }
    return (seq_t)lo;
}

/* --------------------------------------------------------------------- */
/*                          Required public function                       */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* The offset of the sequence is 1, but terms for n < 3 are zero.  */
    if (n < 3) return (seq_t)0;

    /* Ensure the sieve covers n+2 (for the larger twin member). */
    if (sieve_limit < n + 2)
    {
        rebuild_sieve(n + 2);
        rebuild_twin_list();
    }

    /* Count twin primes whose smaller member is ≤ n.  */
    return count_twin_primes(n);
}
