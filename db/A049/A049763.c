/*
 * A049763  Triangular array T, read by rows: T(n,k) = n^4 mod k, for k = 1..n
 *          and n >= 1.
 * https://oeis.org/A049763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A049763.  For a given index n ≥ 1,
   find the row r and column k such that the cumulative count of
   entries up to row r‑1 is < n ≤ the cumulative count up to r.
   Then a(n) = r^4 mod k, where k = n – (r‑1)·r/2.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≥ n.  */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* use 128‑bit arithmetic to avoid overflow */
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                                   /* row index */

    /* Column index k = n minus the number of entries in previous rows */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = (idx_t)(n - prev);                     /* 1 ≤ k ≤ r */

    /* Compute r^4 mod k.  */
    if (k == 1)
        return (seq_t)0;

    seq_t mod  = (seq_t)k;
    seq_t mseq = (seq_t)r;

    /* (r^2 mod k) */
    seq_t r2 = (unsigned __int128)mseq * mseq % mod;
    /* (r^4 mod k) = (r^2)^2 mod k */
    seq_t r4 = (unsigned __int128)r2 * r2 % mod;

    return r4;
}
