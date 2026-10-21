/*
 * A051683  Triangle read by rows: T(n,k) = n!*k.
 * https://oeis.org/A051683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, tabl
 * Formula:   a(n) = r! * k where r is the unique integer with r(r+1)/2 ≥ n
 *            and k = n – r(r‑1)/2 (1 ≤ k ≤ r)
 * Range:     u8 n <= 12, u16 n <= 29, u32 n <= 74, u64 n <= 197,
 *            u128 n <= 562, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Triangle read by rows, T(n,k) = n!*k, offset 1) */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Find the row r such that r(r+1)/2 ≥ n */
    idx_t r = 1;
    while ((__int128)r * (r + 1) / 2 < n)
        ++r;

    /* Number of terms before row r is r(r‑1)/2 */
    idx_t k = n - r * (r - 1) / 2;

    /* Cache factorials up to the current row */
    static seq_t *fact = NULL;
    static idx_t fact_len = 0;   /* current allocation length (valid indices 0..fact_len‑1) */

    if (fact_len <= r) {
        idx_t new_len = r + 1;   /* need indices 0..r inclusive */
        seq_t *tmp = (seq_t *)realloc(fact, new_len * sizeof(seq_t));
        if (!tmp) {             /* allocation failed – return 0 (unlikely in contest) */
            return 0;
        }
        fact = tmp;
        if (fact_len == 0) {
            fact[0] = 1;        /* 0! = 1, not used but convenient */
            fact_len = 1;
        }
        /* Compute missing factorials */
        for (idx_t i = fact_len; i <= r; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_len = new_len;
    }

    /* Result: r! * k */
    return fact[r] * (seq_t)k;
}
