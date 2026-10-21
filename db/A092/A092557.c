/*
 * A092557  Triangle read by rows: T(1,1) = 1; for n>=2, write the first n^2
 *          integers in an n X n array beginning with 1 in the upper left
 *          proceeding left to right and top to bottom; then T(n,k) is the
 *          last prime in the k-th row.
 * https://oeis.org/A092557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A092557
 *  Formula:  a(n) = last prime in the k‑th row of the n×n array
 *  that holds the first n² natural numbers in row‑major order.
 *  The sequence is read rowwise: a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2),
 *  a(4)=T(3,1), a(5)=T(3,2), … .
 *  The code computes the row index r and column k from n, then
 *  scans the interval [(k−1)·r+1 , k·r] for the largest prime.
 *
 *  The implementation uses a simple sieved prime table that is grown
 *  lazily to cover the largest high value needed.  The sieve size is
 *  at most a few tens of thousands for n ≈ 10 000, so the algorithm
 *  is O(n) per query and easily runs under 2 s for the required range.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Prime sieve helpers ------------------------------------ */

static bool  *prime_flag = NULL;   /* prime lookup table */
static size_t prime_limit = 0;     /* largest number currently sieved */

/* Allocate and fill a sieve up to 'limit' (inclusive).  Old table is freed. */
static void init_prime_sieve(size_t limit)
{
    /* If we already cover the range, nothing to do */
    if (prime_flag && prime_limit >= limit) return;

    /* Allocate fresh table */
    bool *new_flag = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!new_flag) return;                    /* allocation failure – unlikely */

    /* Initialise all as true, then mark 0 and 1 as composite */
    for (size_t i = 0; i <= limit; ++i) new_flag[i] = true;
    if (limit >= 0) new_flag[0] = false;
    if (limit >= 1) new_flag[1] = false;

    /* Classic Eratosthenes */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (new_flag[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                new_flag[j] = false;
        }
    }

    /* Replace old table */
    if (prime_flag) free(prime_flag);
    prime_flag  = new_flag;
    prime_limit = limit;
}

/* Find the greatest prime in [low, high].  Assumes high ≤ prime_limit. */
static seq_t last_prime_in_range(seq_t low, seq_t high)
{
    for (seq_t x = high; x >= low; --x) {
        size_t ix = (size_t)x;        /* cast is safe for the ranges we use */
        if (prime_flag[ix]) return x; /* prime found */
    }
    return 0;                         /* Should never happen */
}

/* ---------- Main sequence function --------------------------------- */

seq_t a(idx_t n)
{
    /* special case: the first element of the triangle is defined as 1 */
    if (n == 1) return (seq_t)1;

    /* Find the row r such that r*(r+1)/2 ≥ n */
    idx_t r = 1;
    while ((unsigned __int128)r * (r + 1) / 2 < n) r++;

    /* Number of terms before this row */
    idx_t prev = (idx_t)(((unsigned __int128)r * (r - 1)) / 2);

    /* Column index k within this row (1‑based) */
    idx_t k = n - prev;     /* 1 ≤ k ≤ r */

    /* Interval of numbers in this row */
    seq_t low  = ((seq_t)(k - 1)) * (seq_t)r + 1;
    seq_t high = ((seq_t)k) * (seq_t)r;

    /* Ensure the prime table covers the needed range */
    size_t needed = (size_t)high + 10;        /* small safety margin */
    if (!prime_flag || needed > prime_limit)
        init_prime_sieve(needed);

    /* Return the last prime in the interval */
    return last_prime_in_range(low, high);
}
