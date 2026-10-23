/*
 * A117733  Sum of the n-th primorial and the n-th compositorial number.
 * https://oeis.org/A117733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 26, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A117733: a(n) = primorial(n) + compositorial(n)
   primorial(n)  = product of all primes <= n (empty product = 1)
   compositorial(n) = product of all composite numbers <= n (empty product = 1) */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Helper: sieve of Eratosthenes up to n, returns an array of bools
   where is_prime[i] is true iff i is prime.  The caller must free the
   returned pointer.  For n < 2 the array will contain only false values. */
static bool *sieve(bool *buffer, idx_t n)
{
    bool *is_prime = buffer;
    if (buffer == NULL) {
        size_t sz = (size_t)(n + 1);
        is_prime = (bool *)calloc(sz, sizeof(bool));
    }
    if (is_prime == NULL) return NULL;          /* allocation failure */
    /* initialize all to true except 0,1 */
    for (idx_t i = 0; i <= n; ++i) is_prime[i] = (i >= 2);
    for (idx_t p = 2; (idx_t)((uint64_t)p * p) <= n; ++p) {
        if (is_prime[p]) {
            for (idx_t k = p * p; k <= n; k += p) is_prime[k] = false;
        }
    }
    return is_prime;
}

seq_t a(idx_t n)
{
    if (n < 2) return 2;                /* primorial(0)=1, compositorial(0)=1 */
    /* prime test up to n */
    bool *is_prime = sieve(NULL, n);
    if (is_prime == NULL) return 0;     /* unlikely, treat as 0 */

    seq_t prim = (seq_t)1;
    seq_t comp = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        if (is_prime[i]) {
            prim *= (seq_t)i;
        } else {
            comp *= (seq_t)i;            /* i is composite >1 */
        }
    }
    free(is_prime);
    return prim + comp;
}
