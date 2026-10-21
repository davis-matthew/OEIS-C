/*
 * A204543  Number of (n+1)X2 0..1 arrays with permanents of 2X2 subblocks
 *          differing from neighboring permanents.
 * https://oeis.org/A204543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + 2*a(n-4) (n > 6) Base values: a(1)=16, a(2)=26,
 *            a(3)=41, a(4)=56, a(5)=80, a(6)=112, a(7)=162
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 56, u64 n <= 120, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first six indices directly */
    if (n <= 6) {
        switch (n) {
            case 1: return (seq_t)16;
            case 2: return (seq_t)26;
            case 3: return (seq_t)41;
            case 4: return (seq_t)56;
            case 5: return (seq_t)80;
            case 6: return (seq_t)112;
            default: /* unreachable */ return (seq_t)0;
        }
    }

    /* Recurrence works separately for even and odd indices.
       We compute iteratively without allocating extra storage. */
    if ((n & 1) == 0) {          /* even n */
        seq_t prev4 = (seq_t)56; /* a(4) */
        seq_t prev2 = (seq_t)112;/* a(6) */
        /* compute a(8), a(10), ... up to a(n) */
        for (idx_t i = 8; i <= n; i += 2) {
            seq_t next = prev2 + ((seq_t)2 * prev4);
            prev4 = prev2;
            prev2 = next;
        }
        return prev2;            /* now prev2 holds a(n) */
    } else {                    /* odd n */
        seq_t prev4 = (seq_t)80; /* a(5) */
        seq_t prev2 = (seq_t)162;/* a(7) */
        /* compute a(9), a(11), ... up to a(n) */
        for (idx_t i = 9; i <= n; i += 2) {
            seq_t next = prev2 + ((seq_t)2 * prev4);
            prev4 = prev2;
            prev2 = next;
        }
        return prev2;            /* now prev2 holds a(n) */
    }
}
