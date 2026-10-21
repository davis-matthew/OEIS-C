/*
 * A236312  a(n) = floor((n + e)^2), where e is the natural logarithm base.
 * https://oeis.org/A236312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n + e)^2)
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses fixed‑point arithmetic with 12 decimal digits of e. */
/* The constants are chosen so that the intermediate calculation fits in
   unsigned __int128 for all n <= 10^11; this covers the test ranges. */
seq_t a(idx_t n)
{
    /* 10^12  */
    const unsigned long long SCALE = 1000000000000ULL;
    /* floor(e * 10^12) = 2 718 281 828 459 */
    const unsigned long long E_NUM = 2718281828459ULL;

    /* compute (n + e)^2 = ((n*SCALE + E_NUM)/SCALE)^2 */
    unsigned __int128 t = (unsigned __int128)n * SCALE + E_NUM;
    unsigned __int128 num = t * t;
    unsigned __int128 denom = (unsigned __int128)SCALE * SCALE;

    return (seq_t)(num / denom);
}
