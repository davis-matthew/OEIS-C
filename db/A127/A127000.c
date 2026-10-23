/*
 * A127000  Zero-one fractional-part array for sqrt(2); a rectangular array T
 *          by antidiagonals.
 * https://oeis.org/A127000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 4004, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A127000 – zero‑one fractional‑part array for sqrt(2)
 *  a(n) = ⌊(i+j)√2⌋ – ⌊i√2⌋ – ⌊j√2⌋,
 *  where (i,j) is the antidiagonal pair that contains the nth array entry.
 *  For n ≥ 1:
 *      • Find the smallest s with s·(s–1)/2 ≥ n.
 *      • Let prev = (s–1)·(s–2)/2.
 *      • Let pos = n – prev – 1 (0‑based inside the s‑th diagonal).
 *      • Then i = pos + 1,  j = s – i.
 *  The floor of k·√2 is obtained by ⌊√(2k²)⌋, which uses only integer
 *  arithmetic (implemented with the helper floor_sqrt).
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* Return ⌊k·√2⌋ using integer arithmetic. */
static inline seq_t sqrt2_floor(idx_t k)
{
    seq_t val = (seq_t)k * k * (seq_t)2;   /* val = 2·k² in seq_t   */
    return floor_sqrt(val);               /* ⌊√(val)⌋ = ⌊k·√2⌋       */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* the sequence is defined only for n ≥ 1 */

    /* Find diagonal index s with s·(s–1)/2 ≥ n */
    idx_t s = 2;
    while ((s * (s - 1)) / 2 < n) s++;

    /* Number of entries before this diagonal */
    idx_t prev = (s - 1) * (s - 2) / 2;

    /* Position within the diagonal (0‑based) */
    idx_t pos = n - prev - 1;

    /* Antidiagonal coordinates */
    idx_t i = pos + 1;
    idx_t j = s - i;

    /* Beatty‑sequence values for sqrt(2) */
    seq_t fi   = sqrt2_floor(i);
    seq_t fj   = sqrt2_floor(j);
    seq_t fij  = sqrt2_floor(i + j);

    /* Result is always 0 or 1. */
    return fij - fi - fj;
}
