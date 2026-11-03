/*
 * A026601  Numbers k such that A026600(k) = 1.
 * https://oeis.org/A026601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A079498(n)+1 (positions of 1 in the fixed point of the
 *            morphism 1 → 123, 2 → 231, 3 → 312 )
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9457 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the symbol at position n (1‑based) in A026600 via iterative descent. */
static inline seq_t symbol_at(idx_t n)
{
    seq_t s = 1;                     /* root symbol */
    int stack[64];                   /* stack of remainders; depth < 64 for 64‑bit indices */
    int depth = 0;
    while (n > 1) {
        stack[depth++] = (int)((n - 1) % 3);
        n = (n - 1) / 3 + 1;          /* parent index */
    }
    for (int i = depth - 1; i >= 0; --i) {
        int r = stack[i];
        if (r == 0) {
            /* first symbol of the block: remains unchanged */
            continue;
        } else if (r == 1) {
            /* second symbol: 1→2, 2→3, 3→1 */
            s = (s % 3) + 1;
        } else { /* r == 2 */
            /* third symbol: 1→3, 2→1, 3→2 */
            s = ((s + 1) % 3) + 1;
        }
    }
    return s;
}

/* Return the n‑th term of A026601 (1-indexed). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but guard against zero */
    idx_t count = 0;
    idx_t pos   = 0;
    while (1) {
        ++pos;
        if (symbol_at(pos) == (seq_t)1) {
            ++count;
            if (count == n)
                return (seq_t)pos;
        }
    }
}
