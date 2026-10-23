/*
 * A166514  Zig-zag function for first two columns of a matrix (take numbers
 *          in consecutive pairs).
 * https://oeis.org/A166514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n%2==0) ? n/2 : ((n%4==1)?1:0)
 * Range:     u8 n <= 511, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* even n: return n/2; odd n with n%4==1: return 1; else 0 */
    if ((n & 1ULL) == 0ULL)          /* n even */
        return (seq_t)(n >> 1);
    else if ((n & 3ULL) == 1ULL)     /* n odd, n≡1 (mod 4) */
        return (seq_t)1;
    else                             /* n odd, n≡3 (mod 4) */
        return (seq_t)0;
}
