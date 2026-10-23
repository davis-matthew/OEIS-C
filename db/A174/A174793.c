/*
 * A174793  Triangle read by rows: R(n,k) = n mod 2^Omega(k), where Omega( )
 *          is number of prime divisors counted with multiplicity and 1 <= k
 *          <= n.
 * https://oeis.org/A174793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   bigomega, triangular_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A174793: triangle read by rows.
   a(n) = (row r) mod 2^Ω(k) where  
   r = smallest integer with r(r‑1)/2 < n ≤ r(r+1)/2  
   k = n – r(r‑1)/2. 
   Ω(k) is the total number of prime factors of k counted with multiplicity. */

#include "../utils/bigomega.h"
#include "../utils/triangular_sum.h"

seq_t a(idx_t n)
{
    if (n < 1u)
        return 0;                     /* offset starts at 1 */

    /* Binary search for the smallest r with triangular_sum(r) >= n. */
    idx_t low = 1u, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (triangular_sum(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Total entries before row r. */
    idx_t prev = triangular_sum(r - 1u);

    /* Column index (1‑based) within the row. */
    idx_t k = n - prev;

    /* Ω(k), total prime factor count (with multiplicity). */
    seq_t omega_seq = bigomega(k);
    idx_t omega = (idx_t)omega_seq;   /* fits in idx_t for k ≤ n */

    /* Compute r mod 2^ω(k). */
    if (omega >= 64u) {               /* 2^omega > 2^64, r < 2^64, so r mod 2^omega = r */
        return (seq_t)r;
    }

    idx_t mask = (((idx_t)1) << omega) - 1;
    return (seq_t)(r & mask);
}
