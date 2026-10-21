/*
 * A092556  Triangle read by rows: T(1,1) = 1; for n>=2, write the first n^2
 *          integers in an n X n array beginning with 1 in the upper left
 *          proceeding left to right and top to bottom; then T(n,k) is the
 *          first prime in the k-th row.
 * https://oeis.org/A092556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = first prime appearing in the n-th row of the n×n array
 *            containing the integers 1,…,n² row‑wise
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>

/* Simple deterministic primality test for 64‑bit numbers.  For the
   range required by the problem (n ≤ 10 000 ⇒ n² ≤ 10⁸), trial
   division up to √p is fast enough and fully conforms to the
   constraints. */
static bool isPrime64(uint64_t p)
{
    if (p < 2)                 return false;
    if (p % 2ULL == 0ULL)      return p == 2ULL;
    if (p % 3ULL == 0ULL)      return p == 3ULL;

    uint64_t r = 5ULL;
    while (r * r <= p) {
        if (p % r == 0ULL)     return false;
        if (p % (r + 2ULL) == 0ULL) return false;
        r += 6ULL;
    }
    return true;
}

/* Computes the n-th term of A092556. */
seq_t a(idx_t n)
{
    /* a(1) is defined to be 1.  All other rows contain a prime,
       so we can return 1 safely for the index 1 case. */
    if (n == (idx_t)1) return (seq_t)1;

    /* Determine the size g of the square matrix that contains
       the number at position n.  The first n integers occupy the
       first g rows where g(g+1)/2 ≥ n.  Compute g with a
       floating‑point approximation and correct if necessary. */
    long double td = (long double)8.0L * (long double)n + 1.0L;
    unsigned long long g = (unsigned long long)((sqrt(td) - 1.0L) / 2.0L);
    /* Ensure g(g+1)/2 ≥ n */
    while ((unsigned long long)g * (g + 1ULL) / 2ULL < n) ++g;

    /* The index of the first element of the n-th row within the g × g
       array.  The rows are 1‑based. */
    unsigned long long prev = (unsigned long long)g * (g - 1ULL) / 2ULL;
    unsigned long long k = n - prev;          /* 1‑based row number in g×g */

    /* Compute the integer range for this row.  All numbers are
       represented in a 64‑bit unsigned integer for the search. */
    unsigned long long start = (unsigned long long)(k - 1ULL) * g + 1ULL;
    unsigned long long end   = (unsigned long long)k * g;

    /* Search for the first prime in this range. */
    for (unsigned long long x = start; x <= end; ++x) {
        if (isPrime64(x)) return (seq_t)x;
    }

    /* By the problem statement, there is always a prime in each row.
       This line should never be reached. */
    return (seq_t)1;
}
