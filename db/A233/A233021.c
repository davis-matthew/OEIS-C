/*
 * A233021  Number of n X 3 0..3 arrays with no element x(i,j) adjacent to
 *          value 3-x(i,j) horizontally, vertically, diagonally or
 *          antidiagonally, and top left element zero.
 * https://oeis.org/A233021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A233021
   Empirical recurrence:
   a(n) = 11*a(n-1) - 5*a(n-2) - 11*a(n-3) + 2*a(n-4)
   Initial values:
   a(1) = 9      a(2) = 81      a(3) = 849      a(4) = 8835
   (O(n) evaluation, no dynamic allocation required) */

seq_t a(idx_t n)
{
    /* Sequence is not defined for n = 0, but returning 0 makes the function
       well‑defined for all unsigned indices. */
    if (n == 0)
        return (seq_t)0;

    /* Handle the first four explicitly. */
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)81;
    if (n == 3) return (seq_t)849;
    if (n == 4) return (seq_t)8835;

    /* Store the last four terms in a small rolling buffer. */
    seq_t buf[4];
    buf[0] = (seq_t)9;    /* a(n-4)  */
    buf[1] = (seq_t)81;   /* a(n-3)  */
    buf[2] = (seq_t)849;  /* a(n-2)  */
    buf[3] = (seq_t)8835; /* a(n-1)  */

    /* Compute a(n) by iterating from 5 up to n. */
    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t a1 = buf[3];          /* a(i-1) */
        seq_t a2 = buf[2];          /* a(i-2) */
        seq_t a3 = buf[1];          /* a(i-3) */
        seq_t a4 = buf[0];          /* a(i-4) */

        seq_t next = 11 * a1 - 5 * a2 - 11 * a3 + 2 * a4;

        /* Shift the buffer: drop a(i-4), insert next as a(i-1). */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = next;
    }

    return buf[3]; /* a(n) */
}
