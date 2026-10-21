/*
 * A206113  Number of (n+1) X 4 0..3 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly two counterclockwise and two clockwise edge
 *          increases.
 * https://oeis.org/A206113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 115 * a(n-3) for n > 6 Initial values: a(1) = 6028, a(2)
 *            = 22624, a(3) = 88596, a(4) = 520800, a(5) = 2408480, a(6) =
 *            10792800 The function works for any n >= 1, returning the exact
 *            term as long as the value fits in seq_t
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 23, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for indices 1 … 6 (index 0 unused) */
    static const seq_t base[7] = {
        0,            /* unused for index 0 */
        6028,         /* a(1) */
        22624,        /* a(2) */
        88596,        /* a(3) */
        520800,       /* a(4) */
        2408480,      /* a(5) */
        10792800      /* a(6) */
    };

    if (n <= 6)
        return base[n];

    /* Bring n down to 4, 5, or 6 by repeatedly subtracting 3.
       Count how many steps so we know how many times to multiply by 115. */
    idx_t pos = n;      /* current position (>=7) */
    idx_t steps = 0;    /* number of 3‑step reductions */
    while (pos > 6) {
        pos -= 3;
        steps++;
    }

    seq_t val = base[pos];     /* base value for the reduced index */
    for (idx_t i = 0; i < steps; i++)
        val = val * 115;       /* apply recurrence steps times */

    return val;
}
