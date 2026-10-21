/*
 * A056906  Numbers k such that 36*k^2 + 5 is prime.
 * https://oeis.org/A056906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 15, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that 36*k^2 + 5 is prime
   The function builds a static table incrementally,
   adding each k that satisfies the prime condition.
   No external dependencies beyond the provided is_prime_big
   routine.  This works for all supported seq_t sizes.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* Static storage for found terms – more than enough for the
   required n ≤ 10 000 (≈350 000 k values are enough).  */
#define MAX_TERMS 200000
static seq_t terms[MAX_TERMS];
static size_t terms_len = 0;
static idx_t next_k = 0;

seq_t a(idx_t n)
{
    /* Off‑by‑one: OEIS offset is 1; ignore n==0.  */
    if (n == 0) return 0;

    /* Build the list until we have at least n elements.  */
    while (terms_len < n)
    {
        seq_t kk = (seq_t)next_k;
        seq_t m  = ((seq_t)36) * kk * kk + (seq_t)5;
        if (is_prime_big(m))
            terms[terms_len++] = kk;
        ++next_k;
    }
    return terms[n - 1];
}
