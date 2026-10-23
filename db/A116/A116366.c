/*
 * A116366  Triangle read by rows: even numbers as sums of two odd primes.
 * https://oeis.org/A116366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = prime(n+1)+prime(k+1) (OEIS A116366). a(n) is the * nth
 *            term of the triangle read by rows
 * Range:     u8 n <= 494, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The cumulative number of terms up to row r is r*(r+1)/2.  Find
     * the smallest r such that r*(r+1)/2 >= n.  Then the position
     * inside that row is k = n - (r-1)*r/2.  Finally, a(n) = 
     * prime(r+1) + prime(k+1).  All operations use idx_t and seq_t. */
    if (n == 0)
        return (seq_t)0;          /* Undefined for offset 0, but handled. */

    idx_t row = 1;     /* Current row index (1‑based)       */
    idx_t cum = 1;     /* Cumulative terms up to current row */

    while (cum < n) {
        row++;
        cum += row;
    }

    /* k is 1‑based position within the row. */
    idx_t k = n - (cum - row);

    seq_t prime_row = (seq_t)get_prime(row + 1);
    seq_t prime_k   = (seq_t)get_prime(k + 1);

    return prime_row + prime_k;
}
