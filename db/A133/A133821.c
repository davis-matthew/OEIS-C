/*
 * A133821  Triangle whose rows are sequences of increasing fourth powers: 1;
 *          1,16; 1,16,81; ... .
 * https://oeis.org/A133821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (n - T(r-1))^4 * where T(r) = r*(r+1)/2 and r = smallest
 *            integer with T(r) ≥ n
 * Range:     u8 n <= 9, u16 n <= 135, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Defensive for n==0 – not part of OEIS offset, but return 0. */
    if (n == 0) return 0;

    /* Find row r such that r*(r+1)/2 >= n via binary search.
     * Uses 128‑bit arithmetic to avoid overflow when n is large. */
    idx_t low = 1, high = 1;
    while (( (__uint128_t)high * (high + 1) / 2 ) < n) {
        high <<= 1;  /* double the search bound */
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (( (__uint128_t)mid * (mid + 1) / 2 ) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;   /* row index (1‑based) */

    /* Compute the index within the row: c = n - T(r-1) */
    idx_t t_prev = ( (__uint128_t)(r - 1) * r ) / 2;  /* T(r-1) */
    idx_t c = n - t_prev;                            /* 1‑based column */

    /* Compute c^4 within seq_t type. */
    seq_t base = (seq_t)c;
    seq_t sq   = base * base;      /* c^2 */
    seq_t quart = sq * sq;         /* c^4 */
    return quart;
}
