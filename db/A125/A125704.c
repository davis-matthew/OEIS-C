/*
 * A125704  Table read by antidiagonals: row n contains the positive integers
 *          (in order) which are coprime to the n-th prime.
 * https://oeis.org/A125704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 3355, u16 n <= 9825, u32 n <= 9825, u64 n <= 9825,
 *            u128 n <= 9825, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A125704 (table of integers coprime to nth prime read by antidiagonals).
   The n-th term is obtained by determining the diagonal where the term lies,
   mapping that index to a row (prime index) and column, then applying
   the coprime-to-prime formula: T(n,m) = m-1 + floor((m+prime(n)-2)/(prime(n)-1)). */

#include <stdint.h>
#include "../utils/get_prime.h"   /* get_prime(idx_t k) : returns k-th prime */

static inline idx_t triangle(idx_t s) {
    /* returns s*(s-1)/2 as unsigned __int128 cast to idx_t (safe for all s within idx_t range) */
    return (idx_t)((unsigned __int128)s * (s - 1) / 2);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* out of range, but safe guard */

    /* Find diagonal s such that s*(s-1)/2 >= n */
    idx_t low = 1, high = 2;
    while ((unsigned __int128)high * (high - 1) / 2 < n) high <<= 1;

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid - 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;

    /* Number of terms before this diagonal */
    idx_t prev = triangle(s - 1);
    idx_t offset = n - prev - 1;      /* 0‑based within the diagonal */

    idx_t row = (s - 1) - offset;     /* n (row index) */
    idx_t col = offset + 1;           /* m (column index) */

    /* Compute T(row, col) = col-1 + floor((col+prime-2)/(prime-1)) */
    uint32_t prime = get_prime(row);
    seq_t val = (seq_t)(col - 1)
              + ((seq_t)(col + prime - 2) / (seq_t)(prime - 1));
    return val;
}
