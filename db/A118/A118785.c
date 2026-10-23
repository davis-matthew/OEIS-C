/*
 * A118785  Coordination sequence for 600-cell: number of vertices at graph
 *          distance n from a given vertex on the skeleton of the regular
 *          600-cell (or {3,3,5}).
 * https://oeis.org/A118785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, changed
 * Formula:   a(n) = 1,12,32,42,32,1 for n = 0..5; 0 for n > 5 (certainty from
 *            the 600-cell graph's diameter)
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 5) return (seq_t)0;
    switch ((int)n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)12;
        case 2: return (seq_t)32;
        case 3: return (seq_t)42;
        case 4: return (seq_t)32;
        case 5: return (seq_t)1;
        default: return (seq_t)0;  /* unreachable, but keep it defensive */
    }
}
