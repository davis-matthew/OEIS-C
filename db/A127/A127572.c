/*
 * A127572  Triangle, T(n,k) = sigma(k) * n/k if k|n, T(n,k) = 0 otherwise.
 * https://oeis.org/A127572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A127572 – Triangle T(n,k) = sigma(k) * (n/k) if k|n else 0, 
    flattened row‑wise into a single sequence a(n).  For each n:
        * Find the row r such that r(r‑1)/2 < n ≤ r(r+1)/2
        * Let c = n – r(r‑1)/2   (1‑based column)
        * If r % c != 0 → a(n) = 0
          else a(n) = sigma(c) * (r/c)
    where sigma(x) = sum of all positive divisors of x.  All arithmetic
    is performed in seq_t, converting the index beforehand as required. */

#include <stdint.h>
#include <stdbool.h>

/* Function to compute the sum of divisors sigma(x) for x ≥ 1.
   Uses trial division – sufficient for the small values of c that
   occur in the sequence.  Intermediate multiplies are promoted to
   __int128 to avoid overflow before being cast back to seq_t. */
static seq_t sigma_uint64(uint64_t x)
{
    if (x == 1) return (seq_t)1;
    seq_t result = (seq_t)1;
    uint64_t n = x;
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            seq_t term = (seq_t)1;    /* p^0 */
            seq_t sum  = (seq_t)1;    /* starts with 1 */
            while (n % p == 0)
            {
                n /= p;
                term = (seq_t)((__int128)term * p);
                sum += term;
            }
            result = (seq_t)((__int128)result * sum);
        }
    }
    if (n > 1) /* remaining prime factor */
    {
        result = (seq_t)((__int128)result * ((seq_t)1 + (seq_t)n));
    }
    return result;
}

/* Main sequence function.  For an input index n (≥1) it returns
   the corresponding a(n).  All index arithmetic is done in idx_t
   until the final multiplication, which is performed in seq_t. */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r-1)/2 < n ≤ r*(r+1)/2 */
    idx_t r = 1;
    while ( ( (__int128)r * (r + 1) ) / 2 < n)
        ++r;

    /* Compute the index of the previous row */
    __int128 prev_sum = ( (__int128)r * (r - 1) ) / 2;
    idx_t c = (idx_t)( n - (idx_t)prev_sum );  /* column, 1‑based */

    /* If the element is zero, return 0 immediately */
    if (r % c != 0)
        return (seq_t)0;

    /* Otherwise compute sigma(c) * (r/c) */
    seq_t sigma_c = sigma_uint64( (uint64_t)c );
    seq_t val = sigma_c * ( (seq_t)(r / c) );
    return val;
}
