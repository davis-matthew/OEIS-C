/*
 * A071841  Number of primes == 5 mod 8 <= n.
 * https://oeis.org/A071841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of primes ≤ n that are ≡ 5 (mod 8)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function recomputes the result from scratch each time.  For the
   required size (n up to a few tens of thousands) this is well within
   the time limits and avoids issues with arbitrary‑precision
   arithmetic on dynamic arrays. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static state not needed – recompute on every call */

seq_t a(idx_t n)
{
    /* OEIS starts at offset 1; a(1)=0, a(2)=0, …  */
    if (n <= 2) {
        /* For n == 0 the behaviour is irrelevant; return 0. */
        return (seq_t)0;
    }

    /* Allocate a flag array; 0 = prime candidate, 1 = composite. */
    /* Use char to guarantee 1 byte per flag regardless of the
       C++/C implementation. */
    char *is_comp = (char *)calloc((size_t)n + 1, sizeof(char));
    if (!is_comp) return (seq_t)0;          /* on OOM return 0 (fallback) */

    /* Sieve of Eratosthenes up to n */
    for (idx_t i = 2; i * i <= n; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= n; j += i) {
                is_comp[j] = 1;
            }
        }
    }

    /* Count primes ≡ 5 (mod 8) */
    uint64_t count = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (!is_comp[i] && ((i & 7U) == 5U)) {
            ++count;
        }
    }

    free(is_comp);
    return (seq_t)count;
}
