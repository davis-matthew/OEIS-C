/*
 * A189409  a(n) = prime(n)#^2 + 1, where prime(n)# is the n-th primorial
 *          (A002110).
 * https://oeis.org/A189409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)#^2 + 1, where prime(n)# = product of the first n
 *            primes (with prime(0)# = 1). Off‑by‑zero indexing is used: a(0)
 *            = 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   get_prime, grow_seq_array, seq_alloc_array, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    /* Cache for primorials: P[0] = 1, P[i] = product of first i primes. */
    static seq_t *P = NULL;          /* array of primorials */
    static size_t capacity = 0;      /* current array capacity */
    static idx_t max_cached = 0;     /* highest i for which P[i] is computed */

    if (n == 0)            /* a(0) = 1^2 + 1 = 2 */
        return (seq_t)2;

    /* Ensure array capacity at least n+1 */
    if (capacity < (size_t)(n + 1)) {
        grow_seq_array(&P, &capacity, (size_t)(n + 1));
        if (max_cached == 0)                /* first allocation */
            P[0] = (seq_t)1;
    }

    /* Compute any missing primorials up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t prime_val = (seq_t)get_prime(i);           /* i is 1‑based */
        P[i] = seq_mul(P[i - 1], prime_val);
    }
    max_cached = n;

    /* a(n) = P[n]^2 + 1 */
    seq_t res = seq_mul(P[n], P[n]);
    res += (seq_t)1;
    return res;
}
