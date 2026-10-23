/*
 * A144328  A002260 preceded by a column of 1's: a (1, 1, 2, 3, 4, 5, ...)
 *          crescendo triangle by rows.
 * https://oeis.org/A144328
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

/* 
 * Sequence A144328: triangle of natural numbers with a leading column of 1's,
 * flattened row‑wise.  For index n (1‑based) let t be the row number (t ≥ 1)
 * satisfying (t‑1)*t/2 < n ≤ t*(t+1)/2.  Let pos = n - (t‑1)*t/2.  
 * Then a(n) = 1 if pos == 1 else pos‑1.
 * The function uses only integer arithmetic; no floating point or libm functions.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)    /* not valid for the sequence; return 0 for safety */
        return (seq_t)0;

    /* Find the row t: smallest t with t*(t+1)/2 ≥ n.  Use binary search
     * with 128‑bit arithmetic to avoid overflow. */
    idx_t high = 1;
    while ((__uint128_t)high * (high + 1) / 2 < n)
        high <<= 1;                   /* double until enough */
    idx_t low = high >> 1;

    while (low < high)
    {
        idx_t mid = (low + high) >> 1;
        if ((__uint128_t)mid * (mid + 1) / 2 < n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t t = low;  /* row number */

    /* Position inside the row */
    idx_t tri_prev = (idx_t)((__uint128_t)(t - 1) * t / 2);
    idx_t pos = n - tri_prev;               /* 1 ≤ pos ≤ t */

    if (pos == 1)
        return (seq_t)1;
    else
        return (seq_t)(pos - 1);
}
