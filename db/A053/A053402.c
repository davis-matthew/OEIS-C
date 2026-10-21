/*
 * A053402  Running distances (in meters) in Olympic athletics.
 * https://oeis.org/A053402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A053402 – Running distances (in m) in Olympic athletics
   (offset 1).  The official Olympic race distances are:
        100, 200, 400, 800, 1500, 5000, 10000, 42195
   with no further events.  The first four terms double each time,
   and the remaining terms are the fixed distances listed above. */
seq_t a(idx_t n)
{
    if (n < 1)        /* defined only for n >= 1 */
        return (seq_t)0;

    /* first four terms: 100 * 2^(n-1) */
    if (n <= 4) {
        seq_t base = (seq_t)100;
        seq_t mult = ((seq_t)1) << (n - 1);
        return base * mult;
    }

    /* remaining explicit Olympic distances */
    switch ((unsigned)n) {
        case 5: return (seq_t)1500;
        case 6: return (seq_t)5000;
        case 7: return (seq_t)10000;
        case 8: return (seq_t)42195;
        default: return (seq_t)0;   /* not an official distance */
    }
}
