/*
 * A097288  Rectangular array read by antidiagonals obtained from A097287 by
 *          deleting the zeros.
 * https://oeis.org/A097288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = * Let (i,j) be obtained from n by Cantor’s anti‑diagonal
 *            pairing: * * Find the smallest s with s*(s+1)/2 >= n. * * prev =
 *            (s-1)*s/2 (1≤prev<n≤s*(s+1)/2). * * k = n - prev * * i = k * *
 *            sum = s+1 * * j = sum - i * * If i >= j: * * if j == 1: a(n) =
 *            i*(i+1) * * else: a(n) = i*(i+1) - 2*(i-j+...
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* n≥1 is guaranteed; safety guard */

    /* Binary search for the anti‑diagonal index s:  s*(s+1)/2 ≥ n  */
    idx_t low  = 1;
    idx_t high = n;                 /* T(n)=n(n+1)/2 ≥ n for all n≥1 */

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        /* Use 128‑bit arithmetic to avoid overflow in the comparison */
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;                           /* smallest s with T(s) ≥ n */
    unsigned __int128 prev128 = ((unsigned __int128)(s - 1) * s) / 2;
    idx_t prev = (idx_t)prev128;             /* fits in idx_t, as prev ≤ n */

    idx_t k   = n - prev;                     /* 1‑based index within the diag */
    idx_t i   = k;                            /* row index (1‑based)    */
    idx_t sum = s + 1;                        /* i + j = sum           */
    idx_t j   = sum - i;                      /* column index          */

    seq_t value;

    if (i >= j) {                            /* even positions        */
        if (j == 1) {
            value = (seq_t)i * ((seq_t)i + 1);
        } else {
            value = (seq_t)i * ((seq_t)i + 1) -
                    2 * ((seq_t)(i - j + 1));
        }
    } else { /* i < j : odd positions */     /* odd positions        */
        seq_t start = ((seq_t)(j - 1)) * ((seq_t)(j - 2)) + 1;
        value = start + 2 * ((seq_t)(i - 1));
    }

    return value;
}
