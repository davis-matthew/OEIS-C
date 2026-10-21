/*
 * A202201  Number of (n+2) X 9 binary arrays avoiding patterns 001 and 101 in
 *          rows and columns.
 * https://oeis.org/A202201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+9)*(n+8)*(n+7)*(n+6)*(n+5)*(n+4)*(n+3)*(n+2)^2/2240
 * Range:     u8 none, u16 n <= 3, u32 n <= 22, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Offset 1) */
seq_t a(idx_t n)
{
    /* n is 1‑based; for n == 0 we return 0 (out of range) */
    if (n == 0)
        return 0;

#if defined(__SIZEOF_INT128__)
    /* If seq_t is at most 64 bits, use a 128‑bit intermediate to avoid overflow. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 t = 1;

        t *= ((unsigned __int128)n + 9);
        t *= ((unsigned __int128)n + 8);
        t *= ((unsigned __int128)n + 7);
        t *= ((unsigned __int128)n + 6);
        t *= ((unsigned __int128)n + 5);
        t *= ((unsigned __int128)n + 4);
        t *= ((unsigned __int128)n + 3);

        unsigned __int128 n2 = ((unsigned __int128)n + 2);
        n2 *= n2;                 /* (n+2)^2 */
        t *= n2;

        t /= 2240;                 /* exact division */
        return (seq_t)t;
    }
#endif

    /* Fallback: use seq_t arithmetic.  This works for arbitrary‑precision
       seq_t or for types wider than 64 bits where the result fits. */
    seq_t t = 1;

    t *= (seq_t)(n + 9);
    t *= (seq_t)(n + 8);
    t *= (seq_t)(n + 7);
    t *= (seq_t)(n + 6);
    t *= (seq_t)(n + 5);
    t *= (seq_t)(n + 4);
    t *= (seq_t)(n + 3);

    seq_t n2 = (seq_t)(n + 2);
    n2 *= n2;              /* (n+2)^2 */
    t *= n2;

    t /= 2240;              /* exact division */
    return t;
}
