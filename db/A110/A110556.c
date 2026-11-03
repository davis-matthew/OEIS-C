/*
 * A110556  a(n) = binomial(2*n-1, n)*(-1)^n.
 * https://oeis.org/A110556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = binomial(2*n-1, n) * (-1)^n
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 17, i64 n <= 33, i128 n <= 64,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 978 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */
/* intermediate type: 128 bits at the fixed widths, seq_t for big (where
   __int128 would truncate) */
#ifdef __cplusplus
typedef seq_t wide_u;
typedef seq_t wide_s;
#else
typedef unsigned __int128 wide_u;
typedef __int128 wide_s;
#endif


seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i) {
        /* Use 128-bit intermediate to avoid overflow in 64‑bit builds */
        wide_s tmp = (wide_s)result * (n - 1 + i);
        tmp /= i;            /* exact division */
        result = (seq_t)tmp;
    }
    if (n & 1)   /* odd n ⇒ negative sign */
        return -result;
    else
        return result;
}
