/*
 * A086802  Triangle read by rows in which row n lists (prime(n)-prime(k))/2
 *          for 2 <= k <= n.
 * https://oeis.org/A086802
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, tabl, nonn
 * Formula:   a(n) = (prime(n) – prime(k)) / 2, where k = 2 + ((n – 2) –
 *            T_{r‑1}) and the row r is the smallest integer with T_r > (n –
 *            2), T_j = j*(j-1)/2. The sequence is offset 2 (a(2) is the first
 *            listed term)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

/* triangular number: T(r) = r*(r-1)/2 */
static inline uint64_t triangular(idx_t r)
{
    return (uint64_t)r * (r - 1) / 2;
}

seq_t a(idx_t n)
{
    if (n < 2)            /* below the offset – value irrelevantly 0 */
        return (seq_t)0;

    idx_t idx = n - 2;    /* zero‑based index inside the flattened triangle */

    /* Find the row r that contains this index:
       smallest r with T(r) > idx.  Binary search. */
    idx_t low  = 2;
    idx_t high = idx + 2;   /* safe upper bound for the search */

    while (low < high)
    {
        idx_t mid = (low + high) >> 1;
        if (triangular(mid) <= idx)
            low = mid + 1;
        else
            high = mid;
    }

    idx_t r = low;                      /* row containing the term */
    idx_t prev = triangular(r - 1);     /* terms before this row  */
    idx_t pos  = idx - prev;            /* position within the row (0‑based) */
    idx_t k    = pos + 2;                /* column index (k ≥ 2)    */

    seq_t prime_r = (seq_t)get_prime(r);
    seq_t prime_k = (seq_t)get_prime(k);

    return (seq_t)((prime_r - prime_k) / 2);
}
