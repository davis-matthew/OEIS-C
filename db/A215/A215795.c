/*
 * A215795  Numbers n such that 2^n-1 is a triangular number (A000217).
 * https://oeis.org/A215795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of n such that 2^n-1 is triangular (A215795).
   The known solutions are n = 0, 1, 2, 4, 12. */

/* seq_t and idx_t are defined by the build. */
/* External linkage: a(idx_t n) returns the nth term (1‑based). */
seq_t a(idx_t n)
{
    static const seq_t tbl[] = {0, 1, 2, 4, 12};
    /* Length of the table. */
    idx_t len = (idx_t)(sizeof(tbl) / sizeof(tbl[0]));
    if (n <= len)
        return tbl[n - 1];
    /* The sequence is finite; beyond the known terms we return 0. */
    return (seq_t)0;
}
