/*
 * A091968  Primes congruent to 3 (mod 16).
 * https://oeis.org/A091968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 811, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 3 (mod 16).  a(n) returns the nth such prime. */
/* The implementation builds a simple sieve on-demand and resizes it as
   necessary.  It does not rely on any external helper functions. */

#include <stdlib.h>
#include <string.h>

/* exported function */
seq_t a(idx_t n);

/* --------- static data used by the routine -------------------------------- */

static idx_t  cur_limit   = 0;          /* current sieve limit                */
static size_t prime_cnt  = 0;           /* number of primes stored so far     */
static size_t prime_cap  = 0;           /* capacity of prime array            */
static idx_t *prime_list = NULL;       /* array of primes ≡ 3 (mod 16)       */
static uint8_t *is_comp   = NULL;       /* simple bitset: 1 = composite       */

/* --------- helper: rebuild the sieve up to 'limit' ------------------------ */
static void rebuild_sieve(idx_t limit)
{
    /* Reallocate memory for the sieve bitmap.  Use explicit casts for
       C++ compatibility. */
    if (is_comp) free(is_comp);
    is_comp = (uint8_t *)malloc((size_t)(limit + 1));
    if (!is_comp) exit(EXIT_FAILURE);
    memset(is_comp, 0, (size_t)(limit + 1));
    if (limit >= 0) is_comp[0] = 1;
    if (limit >= 1) is_comp[1] = 1;

    /* Reset the prime list container. */
    if (prime_list) free(prime_list);
    prime_list   = NULL;
    prime_cnt    = 0;
    prime_cap    = 0;

    /* Run the sieve. */
    for (idx_t p = 2; p <= limit; ++p)
    {
        if (!is_comp[p])
        {
            /* Store this prime if it is 3 mod 16. */
            if ( (p & 15) == 3 )
            {
                if (prime_cnt == prime_cap)
                {
                    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                    idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
                    if (!tmp) exit(EXIT_FAILURE);
                    prime_list = tmp;
                    prime_cap = new_cap;
                }
                prime_list[prime_cnt++] = p;
            }

            /* Mark composites in the usual way. */
            if (p <= limit / p)
            {
                for (idx_t q = p * p; q <= limit; q += p)
                    is_comp[q] = 1;
            }
        }
    }
    cur_limit = limit;
}

/* --------- public routine ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n≤0, return 0 for safety */

    /* Ensure we already have at least n primes in the table. */
    if (prime_cnt < n)
    {
        /* Start from a modest bound if we have none yet. */
        idx_t bound = cur_limit ? cur_limit : 20;
        if (bound < 20) bound = 20;

        /* Rebuild the sieve until we have enough primes. */
        while (prime_cnt < n)
        {
            rebuild_sieve(bound);
            if (prime_cnt < n) bound *= 2;
        }
    }

    /* The stored value is an idx_t (uint64_t).  Cast to seq_t for the
       output type. */
    return (seq_t)prime_list[n - 1];
}
