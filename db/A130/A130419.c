/*
 * A130419  Triangle (of n terms per row) where t(n,m) = the m-th positive
 *          integer which is coprime to n!!. (n!! = n*(n-2)*(n-4)..*(2 or
 *          1).).
 * https://oeis.org/A130419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the A130419 sequence.
   For index n (1‑based) the function finds the row r and the position
   m within that row (1‑based).  It then returns the m‑th positive
   integer that is coprime to the double‑factorial r!!.  The search is
   performed by incrementing through the integers, checking divisibility
   by the small set of primes that divide r!!.
   The function works for any value of seq_t and idx_t supplied by the
   build.  It uses only the operations allowed in the specification. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* List of primes up to 200 – sufficient for r ≤ 141 when n ≤ 10000. */
static const uint32_t PRIMES[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
    53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107,
    109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167,
    173, 179, 181, 191, 193, 197, 199
};
static const size_t PRIME_COUNT = sizeof(PRIMES) / sizeof(PRIMES[0]);

/* Return the row r such that triangular(r‑1) < n ≤ triangular(r). */
static idx_t
row_of(idx_t n)
{
    idx_t r = 1;
    idx_t tri = 1;         /* r*(r+1)/2 for r = 1 */
    while (tri < n)
    {
        ++r;
        tri = r * (r + 1) / 2;
    }
    return r;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0)   /* offset is 1, guard against below‑offset calls */
        return (seq_t)0;

    /* Determine row r and column m (1‑based within the row). */
    idx_t r = row_of(n);
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t m = n - prev_tri;          /* m in [1, r] */

    /* Collect primes that divide r!!. */
    uint32_t primes_in[40];           /* enough for r ≤ 141 */
    size_t pc = 0;

    if (r & 1)          /* r is odd – n!! is product of odd numbers */
    {
        for (size_t i = 0; i < PRIME_COUNT && PRIMES[i] <= r; ++i)
        {
            if (PRIMES[i] != 2)          /* 2 does not divide odd n!! */
                primes_in[pc++] = PRIMES[i];
        }
    }
    else                /* r is even – n!! includes all even numbers */
    {
        /* 2 always divides n!! */
        primes_in[pc++] = 2;
        for (size_t i = 0; i < PRIME_COUNT && PRIMES[i] <= r / 2; ++i)
        {
            if (PRIMES[i] != 2)          /* odd primes only */
                primes_in[pc++] = PRIMES[i];
        }
    }

    /* Find the m‑th positive integer coprime to r!!. */
    seq_t count = 0;
    uint64_t x = 1;        /* temp to avoid overflow; r <= 141 */

    while (true)
    {
        bool coprime = true;
        for (size_t i = 0; i < pc; ++i)
        {
            if ((x % (uint64_t)primes_in[i]) == 0)
            {
                coprime = false;
                break;
            }
        }
        if (coprime)
        {
            ++count;
            if (count == m)
                return (seq_t)x;
        }
        ++x;
    }
}
