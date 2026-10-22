/*
 * A003411  Losing initial positions in game: two players alternate in
 *          removing >= 1 stones; last player wins; first player may not
 *          remove all stones; each move <= 3 times previous move.
 * https://oeis.org/A003411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-4), n >= 5; a(0)=1, a(1)=2, a(2)=3, a(3)=4,
 *            a(4)=6
 * Range:     u8 n <= 15, u16 n <= 32, u32 n <= 67, u64 n <= 136,
 *            u128 n <= 273, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t vals[5] = {(seq_t)1, (seq_t)2, (seq_t)3, (seq_t)4, (seq_t)6};
    
    if (n < 5) return vals[n];
    
    for (idx_t i = 5; i <= n; i++) {
        seq_t next = vals[4] + vals[1];
        vals[0] = vals[1];
        vals[1] = vals[2];
        vals[2] = vals[3];
        vals[3] = vals[4];
        vals[4] = next;
    }
    
    return vals[4];
}
