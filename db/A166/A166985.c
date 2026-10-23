/*
 * A166985  Primes of the form phi(n)/2.
 * https://oeis.org/A166985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   calc_phi, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A166985: primes of the form φ(n)/2.
   a(n) = the n‑th prime that can be written as φ(m)/2 for some integer m > 2.
   Additional description: we scan integers m starting at 3, compute
   φ(m) with calc_phi, divide by 2 (bit shift), and record the result
   whenever it is prime (checked by is_prime_big).  Cached results are
   stored so that calls may come in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/calc_phi.h"      /* seq_t calc_phi(seq_t m); */
#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t n); */

static seq_t *prime_cache = NULL;   /* memoised primes */
static size_t cache_len = 0;        /* number of primes stored */
static size_t cache_cap = 0;        /* allocated capacity */
static idx_t last_checked_n = 2;    /* last m that was examined */

static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                    /* critical failure – cannot continue */
    prime_cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* a(0) is undefined; return 0 to signal error */
    if (n == 0)
        return 0;

    /* extend memoised cache until it contains at least n primes */
    while (cache_len < (size_t)n) {
        ++last_checked_n;                    /* next integer to test */
        seq_t phi = calc_phi((seq_t)last_checked_n);
        seq_t half = phi >> 1;               /* φ(n)/2 */
        if (is_prime_big(half)) {
            if (cache_len == cache_cap)
                grow_cache();
            prime_cache[cache_len++] = half;
        }
    }
    return prime_cache[n - 1];
}
