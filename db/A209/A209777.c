/*
 * A209777  Number of q in the range 0<=q<=n for which A006002(n)+A006002(q)
 *          is prime.
 * https://oeis.org/A209777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of q in [0..n] such that A006002(n)+A006002(q) is
 *            prime
 * Range:     u8 n <= 9, u16 n <= 44, u32 n <= 41, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Cached A006002 values:  A006002(k) = k*(k+1)^2/2. */
static seq_t *A_vals = NULL;   /* array of A006002 values */
static size_t  A_cap  = 0;     /* allocated capacity */

/* Ensure that A_vals holds values up to index n (inclusive). */
static void ensure_A_vals(idx_t n)
{
    if ((size_t)n < A_cap)
        return;

    size_t new_cap = (A_cap == 0) ? 1 : A_cap;
    while ((size_t)n >= new_cap)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(A_vals, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* out of memory */

    for (size_t i = A_cap; i < new_cap; ++i)
    {
        seq_t i_s  = (seq_t)i;
        seq_t ip1  = i_s + (seq_t)1;          /* (k+1)   */
        seq_t val  = i_s * ip1 * ip1 / (seq_t)2;  /* k*(k+1)^2/2 */
        tmp[i] = val;
    }

    A_vals = tmp;
    A_cap  = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)      /* offset 1; a(0) is undefined, return 0 as safety */
        return (seq_t)0;

    ensure_A_vals(n);

    seq_t count = 0;
    seq_t An    = A_vals[(size_t)n];

    for (idx_t q = 0; q <= n; ++q)
    {
        seq_t sum = An + A_vals[(size_t)q];
        if (is_prime_big(sum))
            ++count;
    }
    return count;
}
