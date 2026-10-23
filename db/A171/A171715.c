/*
 * A171715  Absolute value of (n-th prime of form 3*m-1 minus n-th prime of
 *          form 3*k+1/2+-1/2).
 * https://oeis.org/A171715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = abs( prime_3m_minus_1(n) - prime_3k_plus_1/2_pm_1/2(n) )
 * Range:     u8 n <= 457, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here prime_3m_minus_1(n) = nth prime ≡5 (mod 6) with 2 as the first element,
   prime_3k_plus_1/2_pm_1/2(n) = nth prime ≡1 (mod 6) with 3 as the first element. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Static storage for the two residue‑class prime lists */
static idx_t *pr1 = NULL;   /* primes ≡5 (mod 6), plus 2 as the first prime */
static idx_t *pr2 = NULL;   /* primes ≡1 (mod 6), plus 3 as the first prime */
static size_t sz1 = 0, sz2 = 0;   /* current lengths of the lists */
static size_t cap1 = 0, cap2 = 0; /* current capacities of the lists */
static idx_t next_candidate = 2;   /* next integer to test for primality */

/* Ensure the array has room for `need` elements. */
static void grow(idx_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need)
        return;
    size_t newcap = (*cap > 0) ? (*cap << 1) : 8;
    while (newcap < need)
        newcap <<= 1;
    *arr = (idx_t*)realloc(*arr, newcap * sizeof(idx_t));
    *cap = newcap;
}

/* Generate enough primes to have at least `n` elements in each list. */
static void generate_up_to(idx_t n)
{
    while (sz1 < n || sz2 < n)
    {
        if (is_prime(next_candidate))
        {
            if (next_candidate == 2 || (next_candidate % 6ULL) == 5ULL)   /* prime ≡5 (mod 6) or 2 */
            {
                grow(&pr1, &cap1, sz1 + 1);
                pr1[sz1++] = next_candidate;
            }
            else if (next_candidate == 3 || (next_candidate % 6ULL) == 1ULL) /* prime ≡1 (mod 6) or 3 */
            {
                grow(&pr2, &cap2, sz2 + 1);
                pr2[sz2++] = next_candidate;
            }
        }
        ++next_candidate;
    }
}

/* Return the OEIS sequence term a(n). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for n=0, return 0 just in case */
    generate_up_to(n);
    idx_t p1 = pr1[n - 1];
    idx_t p2 = pr2[n - 1];
    idx_t diff = (p1 > p2) ? (p1 - p2) : (p2 - p1);
    return (seq_t)diff;
}
