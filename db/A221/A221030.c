/*
 * A221030  Majority value maps: number of n X 3 binary arrays indicating the
 *          locations of corresponding elements equal to at least half of
 *          their horizontal and antidiagonal neighbors in a random 0..2 n X 3
 *          array.
 * https://oeis.org/A221030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 8*a(n-1) - 8*a(n-2) - a(n-3) + 5*a(n-4) - 2*a(n-5)
   Initial values:
     a(1) = 4,   a(2) = 33,   a(3) = 228,   a(4) = 1561,   a(5) = 10648 */
 
seq_t a(idx_t n)
{
    /* Undefined for non‑positive n – return 0 as a safe sentinel */
    if (n < 1)
        return 0;

    /* Handle the first five terms directly */
    if (n == 1) return 4;
    if (n == 2) return 33;
    if (n == 3) return 228;
    if (n == 4) return 1561;
    if (n == 5) return 10648;

    /* Use a rolling window of the last five values to compute up to n */
    seq_t a1 = 4;      /* a(n-5) */
    seq_t a2 = 33;     /* a(n-4) */
    seq_t a3 = 228;    /* a(n-3) */
    seq_t a4 = 1561;   /* a(n-2) */
    seq_t a5 = 10648;  /* a(n-1) */

    for (idx_t i = 6; i <= n; ++i)
    {
        seq_t next = 8 * a5 - 8 * a4 - a3 + 5 * a2 - 2 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    return a5;
}
