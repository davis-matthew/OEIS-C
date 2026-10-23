/*
 * A109488  Weight distribution of [64,18,22] extended binary primitive BCH
 *          (or XBCH) code.
 * https://oeis.org/A109488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of codewords of weight 2n in the [64,18,22]
 *            extended binary primitive BCH code. The sequence is defined for
 *            n ≥ 0 with offset 0; a(n) = 0 for all n > 32
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* weight distribution for n = weight/2 (0 ≤ n ≤ 32) */
    static const seq_t dist[33] = {
        /*  0 */ 1ULL,
        /*  1-10 */ 0,0,0,0,0,0,0,0,0,0,
        /* 11-21 */ 4224ULL,5040ULL,24192ULL,12544ULL,69888ULL,
                      30366ULL,69888ULL,12544ULL,24192ULL,5040ULL,4224ULL,
        /* 22-31  */ 0,0,0,0,0,0,0,0,0,0,
        /* 32 */ 1ULL
    };
    return (n < 33) ? dist[n] : (seq_t)0;
}
