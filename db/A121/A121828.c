/*
 * A121828  Ceiling((Pi-e)n).
 * https://oeis.org/A121828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((Pi-e)*n)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t A   = (seq_t)423310825130748ULL;          /* (Pi-e) * 10^15 rounded down */
static const seq_t DEN = (seq_t)1000000000000000ULL;        /* 10^15 */

seq_t a(idx_t n)
{
    /* Ensure n >= 1 (the sequence is defined for n>=1). */
    seq_t un = (seq_t)n;
    seq_t prod = un * A;                 /* (Pi-e)*10^15 * n */
    return (prod + DEN - 1) / DEN;       /* ceil division */
}
