/*
 * A038529  n-th prime - n-th composite.
 * https://oeis.org/A038529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = prime(n) - composite(n) (n‑th composite is the n‑th
 *            composite number > 1, i.e. 4, 6, 8, 9, 10, …)
 * Range:     i8 n <= 42, i16 n <= 3954, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* We keep global static buffers that grow as needed.
   They are thread‑safe only if all calls are from the same thread.  */

static idx_t *primes   = NULL;
static idx_t *composites = NULL;
static size_t primes_cnt   = 0;
static size_t composites_cnt = 0;
static size_t primes_cap   = 0;
static size_t composites_cap = 0;
static idx_t next_num = 2;            /* next integer to test */

/* Helper to grow an array when it reaches capacity. */
static void grow_cap(size_t *cap, void **arr, size_t size)
{
    if (*cap == 0)
        *cap = 16;
    else
        *cap *= 2;
    *arr = realloc(*arr, *cap * size);
}

/* Ensure that we have at least n primes and n composites computed. */
static void ensure(idx_t n)
{
    if (primes_cnt >= (size_t)n && composites_cnt >= (size_t)n)
        return;

    while (primes_cnt < (size_t)n || composites_cnt < (size_t)n) {
        /* Skip 1: it is neither prime nor composite for our definition. */
        if (next_num == 1) {
            next_num++;
            continue;
        }

        if (is_prime(next_num)) {
            if (primes_cnt == primes_cap)
                grow_cap(&primes_cap, (void **)&primes, sizeof(idx_t));
            primes[primes_cnt++] = next_num;
        } else {
            if (composites_cnt == composites_cap)
                grow_cap(&composites_cap, (void **)&composites, sizeof(idx_t));
            composites[composites_cnt++] = next_num;
        }
        next_num++;
    }
}

/* Main OEIS function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset is 1; behave reasonably at n=0 */

    ensure(n);

    /* nth prime and nth composite are stored as 0‑based indices */
    seq_t prime_n = (seq_t)primes[n - 1];
    seq_t composite_n = (seq_t)composites[n - 1];
    return prime_n - composite_n;
}
