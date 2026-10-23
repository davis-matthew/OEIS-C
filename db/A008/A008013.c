/*
 * A008013  Coordination sequence occurring in Zeolite Codes AFG, CAN, LIO,
 *          LOS.
 * https://oeis.org/A008013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(5m) = 52*m² + 2, a(5m+1) = 52*m² + 22*m + 4, a(5m+2) = 52*m² +
 *            42*m + 10, a(5m+3) = 52*m² + 62*m + 20, a(5m+4) = 52*m² + 82*m +
 *            34
 * Range:     u8 n <= 10, u16 n <= 177, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for OEIS sequence A008013 */
seq_t a(idx_t n)
{
    if (n == 0)                /* a(0) = 1 (exception to the 5m formula) */
        return 1;

    idx_t m = n / 5;
    idx_t r = n % 5;

    seq_t sm   = (seq_t)m;
    seq_t base = (seq_t)52 * sm * sm;

    switch (r) {
        case 0: return base + 2;
        case 1: return base + (seq_t)22 * sm + 4;
        case 2: return base + (seq_t)42 * sm + 10;
        case 3: return base + (seq_t)62 * sm + 20;
        case 4: return base + (seq_t)82 * sm + 34;
        default: return 0;   /* unreachable */
    }
}
