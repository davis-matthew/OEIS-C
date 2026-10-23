/*
 * A121255  Number of conjugated cycles composed of ten carbons in
 *          (n,n)-nanotubes in terms of the number of naphthalene units.
 * https://oeis.org/A121255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Conjecture: a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3) - a(n-4)
   with initial values a(1)=0, a(2)=8, a(3)=36, a(4)=128 */
seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)36;
    if (n == 4) return (seq_t)128;

    /* Sliding window for the recurrence. */
    seq_t a1 = (seq_t)128; /* a(4) */
    seq_t a2 = (seq_t)36;  /* a(3) */
    seq_t a3 = (seq_t)8;   /* a(2) */
    seq_t a4 = (seq_t)0;   /* a(1) */
    seq_t ak;

    for (idx_t i = 5; i <= n; ++i) {
        ak = (seq_t)6 * a1 - (seq_t)11 * a2 + (seq_t)6 * a3 - a4;
        /* Shift the window for the next iteration. */
        a4 = a3;
        a3 = a2;
        a2 = a1;
        a1 = ak;
    }
    return ak;
}
