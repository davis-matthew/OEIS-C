/*
 * A204939  Least k such that n divides A130328(k-1), the k-th difference
 *          between numbers of the form 2^(k-1).
 * https://oeis.org/A204939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k >= 1 such that n | A130328(k-1), where A130328 is
 *            read by rows t(r,c) = 2^(r+1) - 2^c, 0 <= c <= r. The code finds
 *            it by a row-by-row search, comparing powers of 2 mod n
 * Range:     u8 n <= 28, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 m = 0;          /* index into A130328 */
    unsigned __int128 hi = 2 % N;     /* 2^(r+1) mod n for r = 0 */
    for (;;) {
        unsigned __int128 lo = 1 % N; /* 2^c mod n */
        /* row r has entries c = 0..r; entry is 2^(r+1) - 2^c */
        for (;;) {
            if (lo == hi) {
                return (seq_t)(m + 1);
            }
            ++m;
            lo = (lo * 2) % N;
            if (lo == hi) {
                /* this is c+1; only valid if c+1 <= r, handled below */
            }
            /* Row-end detection is done via counter below */
            break;
        }
        /* The loop above is replaced by explicit row iteration: */
        (void)lo;
        break;
    }

    /* Explicit row/column iteration */
    m = 0;
    hi = 2 % N;
    for (unsigned __int128 r = 0;; ++r) {
        unsigned __int128 lo = 1 % N;
        for (unsigned __int128 c = 0; c <= r; ++c) {
            if (lo == hi) {
                return (seq_t)(m + 1);
            }
            ++m;
            lo = (lo * 2) % N;
        }
        hi = (hi * 2) % N;
    }
}
