/*
 * A105644  a(n) = floor((Pi+e)*n).
 * https://oeis.org/A105644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((pi+e)*n)
 * Range:     u8 n <= 43, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 1<<60 is used as the scaling factor. */
    static const __int128 SCALE = (( __int128)1ULL << 60);

    /*
     * Use 15–decimal approximations of pi and e:
     *     pi  ≈ 3.141592653589793
     *     e   ≈ 2.718281828459045
     * Their sum with 15 decimals after the point is
     *     5.859874482048838
     *
     * Represent this value as an integer NUM divided by 10^15:
     *     NUM = 5.859874482048838 * 10^15 = 5859874482048838.
     *
     * Compute floor((pi+e)*SCALE) exactly using integer arithmetic.
     */
    static const __int128 NUM = (__int128)5859874482048838ULL;   /* (pi+e)*1e15 */
    static const __int128 K   = (NUM * SCALE) / 1000000000000000ULL; /* floor((pi+e)*SCALE) */

    /* Compute floor((pi+e)*n) as (K*n) >> 60. */
    __int128 prod = K * (__int128)n;
    prod >>= 60;
    return (seq_t)prod;
}
