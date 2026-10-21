/*
 * A208046  Number of n X 4 0..3 arrays with new values 0..3 introduced in row
 *          major order and no element equal to any horizontal, vertical or
 *          antidiagonal neighbor (colorings ignoring permutations of colors).
 * https://oeis.org/A208046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 40*a(n-2) + 68*a(n-3) - 64*a(n-4) (n > 5)
 *            a(1) = 5, a(2) = 32, a(3) = 244, a(4) = 1904, a(5) = 14976
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. Return 0 for any smaller index. */
    if (n < 1)
        return (seq_t)0;

    /* Pre‑defined first five values. element 0 is unused to keep indexes 1‑based. */
    static const seq_t base[6] = {
        0,                         /* unused (index 0) */
        (seq_t)5,   /* a(1) */
        (seq_t)32,  /* a(2) */
        (seq_t)244, /* a(3) */
        (seq_t)1904,/* a(4) */
        (seq_t)14976/* a(5) */
    };

    if (n <= 5)
        return base[n];

    /* Initialise the window for the recurrence:
       a1 = a(n-4), a2 = a(n-3), a3 = a(n-2), a4 = a(n-1)   for n = 6 */
    seq_t a1 = base[2];  /* a(2) */
    seq_t a2 = base[3];  /* a(3) */
    seq_t a3 = base[4];  /* a(4) */
    seq_t a4 = base[5];  /* a(5) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = (seq_t)12 * a4 - (seq_t)40 * a3 +
                     (seq_t)68 * a2 - (seq_t)64 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
