/*
 * A034114  Fractional part of square root of a(n) starts with 8: first term
 *          of runs.
 * https://oeis.org/A034114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ceil((n + 1.8)^2) = ceil((25*n^2 + 90*n + 81)/25)
 * Range:     u8 n <= 1, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A034114: Fractional part of sqrt(a(n)) starts with 8: first term of runs.   */

seq_t a(idx_t n)
{
    /* n ≥ 1 according to the offset.  */
    seq_t nn = (seq_t)n;                    /* promote to seq_t for safety */
    seq_t A  = 25 * nn * nn + 90 * nn + 81; /* numerator of the rational expression  */
    return (A + 24) / 25;                  /* ceil(A/25)  */
}
