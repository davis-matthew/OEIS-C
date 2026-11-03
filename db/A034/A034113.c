/*
 * A034113  Fractional part of square root of a(n) starts with 7: first term
 *          of runs.
 * https://oeis.org/A034113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the first integer of each run of consecutive integers k
 *            for which the fractional part of sqrt(k) starts with digit 7.
 *            Equivalently, let k >= 1 and set L(k) = ceil((k + 0.7)^2) U(k) =
 *            floor((k + 0.8)^2) Then a(n) = L(k) for the n‑th value of k with
 *            L(k) <= U(k)
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline seq_t run_first_term(idx_t k)
{
    /* Compute L(k) = ceil((k+0.7)^2) and U(k) = floor((k+0.8)^2) */
    unsigned __int128 k10 = (unsigned __int128)k * 10;   /* 10*k */
    unsigned __int128 t1   = k10 + 7;                    /* 10*k + 7   */
    unsigned __int128 t2   = k10 + 8;                    /* 10*k + 8   */

    unsigned __int128 tl   = t1 * t1 + 99;               /* ceil: +99/100 */
    unsigned __int128 tu   = t2 * t2;                   /* floor: /100 */

    seq_t L = (seq_t)(tl / 100);
    seq_t U = (seq_t)(tu / 100);

    return (L <= U) ? L : 0ULL;      /* return L if run exists, 0 otherwise */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset 0, but guard */

    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t first = run_first_term(k);
        if (first != 0) {
            ++count;
            if (count == n) return first;
        }
    }
}
