/*
 * A086355  Fixed point if [nonzero-digit product]-function at
 *          initial-value=prime(n) is iterated.
 * https://oeis.org/A086355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = fixed point of the non‑zero digit product * of prime(n)
 *            (i.e. A051802(A000040(n)))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_prod_nonzero, ensure_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_prod_nonzero.h"
#include "../utils/ensure_primes.h"

/* Sequence definition */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    uint64_t *prime_list = ensure_primes(n);
    if (!prime_list) return (seq_t)0;     /* allocation error – abort silently */

    uint64_t cur = prime_list[n - 1];     /* nth prime (1‑based) */

    /* iterate digit product until a fixed point is reached */
    for (;;) {
        uint64_t next = digit_prod_nonzero(cur);
        if (next == cur) break;
        cur = next;
    }
    return (seq_t)cur;                     /* final fixed point is a small value */
}
