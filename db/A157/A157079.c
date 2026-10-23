/*
 * A157079  a(n) = 32805000*n^2 - 10513800*n + 842401.
 * https://oeis.org/A157079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32805000*n^2 - 10513800*n + 842401
 * Range:     u8 none, u16 none, u32 n <= 11, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 23133601, a(2) = 111034801, ... */

seq_t a(idx_t n)
{
    /* Convert index to seq_t to avoid accidental 64‑bit promotion of the
       integer constants.  All arithmetic is done in seq_t, which is the
       desired width for the current build. */
    seq_t nn = (seq_t)n;

    /* Compute the quadratic formula using the given constant coefficients. */
    return (seq_t)32805000 * nn * nn
           - (seq_t)10513800 * nn
           + (seq_t)842401;
}
