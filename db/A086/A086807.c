/*
 * A086807  Erroneous version of A000213.
 * https://oeis.org/A086807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 9, i32 n <= 9, i64 n <= 9, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes that are 1 mod 4, with 2 as the first element. */
#include <stdlib.h>
#include <math.h>

static size_t estimate_limit(size_t n)
{
    /* Rough upper bound on the n-th prime of the form 4k+1.
       We need about twice as many ordinary primes. */
    size_t m = n * 2;
    if (m < 6) m = 6;
    double d = (double)m;
    double upper = d * (log(d) + log(log(d))) + 3.0 * d;
    return (size_t)upper + 10;           /* safety margin */
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n <= 0 */

    /* Static storage persisting between calls. */
    static seq_t *list          = NULL;
    static size_t list_size     = 0;
    static size_t list_cap      = 0;
    static size_t sieve_limit   = 0;
    static uint8_t *sieve       = NULL;

    /* Rebuild if vector is too small. */
    if ((size_t)n > list_size)
    {
        size_t limit = estimate_limit((size_t)n);

        /* Rebuild the sieve. */
        if (sieve) free(sieve);
        sieve = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
        if (!sieve) return 0;           /* allocation failure */

        /* initialise as prime, except 0 and 1 */
        for (size_t i = 0; i <= limit; ++i) sieve[i] = 1;
        sieve[0] = sieve[1] = 0;

        size_t sq = (size_t)sqrt((double)limit);
        for (size_t p = 2; p <= sq; ++p)
        {
            if (sieve[p])
            {
                for (size_t k = p * p; k <= limit; k += p)
                    sieve[k] = 0;
            }
        }

        /* Build the list of 2 or 1 mod 4 primes. */
        if (list) free(list);
        list_cap = limit / 10 + 10;          /* rough estimate */
        list = (seq_t*)malloc(list_cap * sizeof(seq_t));
        if (!list) { free(sieve); return 0; }  /* allocation failure */

        list_size = 0;
        for (size_t i = 2; i <= limit; ++i)
        {
            if (sieve[i] && (i == 2 || (i % 4 == 1)))
            {
                if (list_size == list_cap)
                {
                    list_cap = list_cap + list_cap / 2 + 10;
                    seq_t *tmp = (seq_t*)realloc(list, list_cap * sizeof(seq_t));
                    if (!tmp) { free(list); free(sieve); return 0; }
                    list = tmp;
                }
                list[list_size++] = (seq_t)i;
            }
        }
        sieve_limit = limit;
    }

    /* Now list_size >= n. */
    return list[n - 1];
}
