/*
 * A070990  First differences of A002487.
 * https://oeis.org/A070990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(2n) = S(n+2), a(2n+1) = -S((n+1)/2) where S(k) is Stern's
 *            diatomic sequence (A002487): S(0) = 0, S(1) = 1, S(2k) = S(k),
 *            S(2k+1) = S(k) + S(k+1)
 * Range:     i8 n <= 99, i16 n <= 99, i32 n <= 99, i64 n <= 99, i128 n <= 99,
 *            big any n
 * Validated: partial, pass, 100 terms through n = 99 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute Stern's diatomic value S(k) recursively.
   The depth of recursion is O(log k), no additional storage is required. */
static seq_t stern_diatomic(idx_t k)
{
    if (k == 0) return (seq_t)0;
    if (k == 1) return (seq_t)1;
    if ((k & 1) == 0)  /* k even */
        return stern_diatomic(k >> 1);
    else               /* k odd  */
        return stern_diatomic(k >> 1) + stern_diatomic((k >> 1) + 1);
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if ((n & 1) == 0) {            /* even index: a(n) = S(n/2 + 2) */
        idx_t k = (n >> 1) + 2;    /* n/2 + 2 without overflow */
        return stern_diatomic(k);
    } else {                       /* odd index: a(n) = -S((n+1)/2) */
        idx_t k = (n + 1) >> 1;    /* (n+1)/2 */
        return -(stern_diatomic(k));
    }
}
