/*
 * A166479  Lesser of twin primes, written in base 6.
 * https://oeis.org/A166479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = A007092(A001359(n)) (the n‑th lesser twin prime written
 *            in base 6)
 * Range:     u8 n <= 10, u16 n <= 173, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc, seq_alloc_array, seq_free, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/seq_alloc.h"
#include "../utils/seq_free.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

/* Cached list of lower twin primes.  The array holds seq_t values and is
   reallocated whenever we grow the sieve range.  The cache persists
   across calls. */
static seq_t *twin_list   = NULL;   /* array of lower twin primes */
static idx_t twin_list_size = 0;    /* number of primes stored */
static idx_t prime_limit   = 0;     /* current sieve upper bound */

/* Rebuild the twin‑prime table until it contains at least n elements.  The
   sieve range is doubled each time more primes are needed.  The function
   uses only the helper allocate/free functions so it works for the
   arbitrary‑precision build as well. */
static void
update_twin_list_until(idx_t n)
{
    while (twin_list_size < n) {
        /* set a new, larger sieve limit */
        prime_limit = (prime_limit == 0) ? 1000 : prime_limit * 2;

        /* 1‑based array of flags: 1 = prime, 0 = composite */
        unsigned char *flag = (unsigned char *)malloc(prime_limit + 1);
        if (!flag) return;                /* allocation failure – abort */

        for (idx_t i = 0; i <= prime_limit; ++i)
            flag[i] = 1;
        flag[0] = flag[1] = 0;

        /* classic Eratosthenes sieve */
        for (idx_t i = 2; i * i <= prime_limit; ++i) {
            if (flag[i]) {
                for (idx_t j = i * i; j <= prime_limit; j += i)
                    flag[j] = 0;
            }
        }

        /* estimate a generous capacity for the new list */
        idx_t capacity = prime_limit / 2 + 10;
        seq_t *new_list = seq_alloc_array(capacity);
        if (!new_list) { free(flag); return; }

        idx_t count = 0;
        for (idx_t p = 2; p + 2 <= prime_limit; ++p) {
            if (flag[p] && flag[p + 2]) {
                new_list[count++] = (seq_t)p;
            }
        }

        /* replace the old list */
        if (twin_list)
            seq_free_array(twin_list);
        twin_list = new_list;
        twin_list_size = count;

        free(flag);
    }
}

/* Public function required by the build system.  It returns the base‑6
   representation (as a decimal integer) of the nth lesser twin prime.
   Index n is 1‑based; a(1) = 3.  The function is safe to call in any order
   and caches intermediate results. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* below the offset */

    /* Ensure that the cache contains at least n twin primes. */
    if (twin_list_size < n)
        update_twin_list_until(n);

    seq_t prime = twin_list[n - 1];            /* nth lower twin prime */

    /* Convert the prime to base 6 and then to a decimal representation. */
    seq_t value = 0;
    seq_t tmp   = prime;

    /* Enough space for a very large number of base‑6 digits. */
    unsigned char digits[1024];
    int dcnt = 0;

    if (tmp == 0) {
        digits[dcnt++] = 0;
    } else {
        while (tmp > 0) {
            digits[dcnt++] = (unsigned char)(tmp % 6);
            tmp /= 6;
        }
    }

    for (int i = dcnt - 1; i >= 0; --i) {
        value = value * 10 + digits[i];
    }

    return value;
}
