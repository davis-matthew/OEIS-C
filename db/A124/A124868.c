/*
 * A124868  Natural numbers that are not the sum of 3 distinct primes.
 * https://oeis.org/A124868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A124868: natural numbers that are NOT the sum of three distinct primes. 
   For an index n ≥ 1, a(n) is the n‑th such natural number. 
   The implementation tests each integer sequentially until it has found
   n terms that cannot be expressed as p+q+r with distinct primes p<q<r. */

#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t x) */

/* Returns true if x can be written as the sum of three distinct primes p<q<r. */
static bool is_sum_of_three_distinct_primes(idx_t x)
{
    /* The smallest sum of three distinct primes is 2+3+5 = 10. */
    if (x < 10)
        return false;

    /* p cannot exceed n/3 because p < q < r and p+q+r = n. */
    for (idx_t p = 2; p <= x / 3; ++p) {
        if (!is_prime(p))
            continue;

        /* q must be greater than p and at most (n-p)/2. */
        for (idx_t q = p + 1; q <= (x - p) / 2; ++q) {
            if (!is_prime(q))
                continue;

            idx_t r = x - p - q;
            if (r <= q)    /* enforce p < q < r */
                break;

            if (is_prime(r))
                return true;   /* found a valid representation */
        }
    }
    return false;  /* no representation found */
}

/* Return the n-th term of the sequence, with offset 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but avoid UB for off‑by‑one callers */

    idx_t curr = 1;     /* candidate natural number */
    idx_t count = 0;    /* number of terms found so far */

    while (count < n) {
        if (!is_sum_of_three_distinct_primes(curr)) {
            ++count;
            if (count == n)
                return (seq_t)curr;
        }
        ++curr;
    }

    /* The loop will always return before reaching this point. */
    return (seq_t)curr;
}
