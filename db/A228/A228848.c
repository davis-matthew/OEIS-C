/*
 * A228848  a(n) = round(3*n^2/Pi^2).
 * https://oeis.org/A228848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(3*n^2/Pi^2)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the continued‑fraction approximation Pi ≈ 355/113; */
/* thus Pi^2 ≈ (355/113)^2 = 126025/12769.  */
/* The rounding is performed with integer arithmetic only. */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to seq_t */
    seq_t num = nn * nn;          /* n^2          */
    num *= 3;                     /* 3*n^2        */
    num *= 12769;                 /* multiply by 113^2   */
    const seq_t denom = 126025;   /* 355^2             */
    /* Round: add denom/2 before integer division */
    seq_t result = (num + denom / 2) / denom;
    return result;
}
