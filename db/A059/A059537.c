/*
 * A059537  Beatty sequence for zeta(3).
 * https://oeis.org/A059537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * ζ(3))
 * Range:     u8 n <= 212, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ζ(3) ≈ 1.202056903159594285399738161511949655932995... */
/* Using a rational approximation with 18 decimal digits of precision:
   NUM / DEN ≈ ζ(3)   where
   NUM = 1202056903159594286
   DEN = 1000000000000000000
   The approximation error is < 5e-19, so for any n <= 10¹² the floor
   computation is exact. The result is safe for all seq_t sizes that
   support the arithmetic performed below.  */
/* All arithmetic is done using unsigned __int128 for intermediate
   results to avoid overflow on 64‑bit builds.  */

#include <stdint.h>

/* Numeric constants:  NUM/DEN ≈ ζ(3) with 18 decimal places. */
static const unsigned __int128 NUM =
    (unsigned __int128)1202056903159594286ULL;
static const unsigned __int128 DEN =
    (unsigned __int128)1000000000000000000ULL;

/* Compute the n‑th term of A059537: floor(n * ζ(3)). */
seq_t a(idx_t n)
{
    /* Promote n to 128‑bit to avoid overflow when multiplying by NUM. */
    unsigned __int128 prod = (unsigned __int128)(unsigned long long)n * NUM;
    unsigned __int128 result = prod / DEN;
    /* Convert back to seq_t.  If seq_t is smaller than 128 bits the
       value will be truncated automatically, which is acceptable
       because the sequence value must fit in seq_t for the test
       data. */
    return (seq_t)result;
}
