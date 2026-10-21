/*
 * A227736  Irregular table read by rows: the first entry of n-th row is
 *          length of run of rightmost identical bits (either 0 or 1, equal to
 *          n mod 2), followed by length of the next run of bits, etc., in the
 *          binary representation of n, when scanned from the least
 *          significant to the most significant end.
 * https://oeis.org/A227736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf, changed
 * Formula:   a(n) enumerates the run‑lengths of the binary representations of
 *            * 1,2,3,... reading the rows left to right. Each row for integer
 *            k consists * of the lengths of consecutive runs of identical
 *            bits scanned from least * significant to most significant. The
 *            function returns the n‑th run‑length
 * Range:     u8 n <= 5120, u16 n <= 5120, u32 n <= 5120, u64 n <= 5120,
 *            u128 n <= 5120, big any n
 * Helpers:   run_length_at
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/run_length_at.h"

/* popcount for 64‑bit unsigned integers */
static inline idx_t popcount64(idx_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll((unsigned long long)x);
#else
    idx_t c = 0;
    while (x) { c += x & 1; x >>= 1; }
    return c;
#endif
}

seq_t a(idx_t n)
{
    if (n == 0)                    /* offset of sequence is 1              */
        return (seq_t)0;

    idx_t pos = 0;                  /* cumulative run count so far          */

    for (idx_t k = 1;; ++k) {
        /* Number of runs in binary of k */
        idx_t runs = popcount64(k ^ (k >> 1));

        if (pos + runs >= n) {
            /* Target row found */
            idx_t off = n - pos - 1;            /* 0‑based index within row     */
            return run_length_at(k, off);
        }
        pos += runs;
    }
}
