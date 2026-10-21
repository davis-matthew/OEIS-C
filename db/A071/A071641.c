/*
 * A071641  a(n) defined by recursion in the formula section.
 * https://oeis.org/A071641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 10 - (prime(g(n)) mod 10) where g(n) = 9 - (f(n) mod 9)
 *            f(n) = f(n-1) + f(n-4) + floor((f(n-1)+f(n-4))/10), f(k) = 1 for
 *            k < 4
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 62, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *f_cache   = NULL;   /* cache for f(0)..f(n)   */
static idx_t  f_size    = 0;      /* number of elements computed  */
static idx_t  f_capacity= 0;      /* current allocation capacity */

seq_t a(idx_t n)
{
    /* Expand the cache if needed */
    if (n >= f_capacity) {
        idx_t new_cap = (n + 1) * 2;            /* grow twice as fast */
        seq_t *tmp = (seq_t *)realloc(f_cache, sizeof(seq_t) * new_cap);
        if (!tmp) abort();                     /* allocation failure */
        f_cache    = tmp;
        f_capacity = new_cap;
    }

    /* Compute f(i) for all i up to n that are not yet known */
    for (idx_t i = f_size; i <= n; ++i) {
        if (i < 4) {
            f_cache[i] = (seq_t)1;
        } else {
            seq_t a = f_cache[i-1];
            seq_t b = f_cache[i-4];
            seq_t sum   = a + b;
            seq_t carry = sum / (seq_t)10;
            f_cache[i]  = sum + carry;
        }
    }
    if (n >= f_size) f_size = n + 1;

    /* g(n) = 9 - (f(n) mod 9)  →  1..9 */
    idx_t g = 9 - (idx_t)(f_cache[n] % (seq_t)9);

    /* 9-th primes (1‑based index) */
    static const seq_t primes[9] = {
        (seq_t)2,  (seq_t)3,  (seq_t)5,  (seq_t)7,
        (seq_t)11, (seq_t)13, (seq_t)17, (seq_t)19,
        (seq_t)23
    };

    seq_t prime_val = primes[g-1];
    seq_t mod10     = prime_val % (seq_t)10;
    seq_t result    = (seq_t)10 - mod10;
    return result;
}
