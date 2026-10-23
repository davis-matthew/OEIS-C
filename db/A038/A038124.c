/*
 * A038124  Beatty sequence for Brun's constant.
 * https://oeis.org/A038124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*A065421)
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A065421  = 1.902160583104…   (Brun's constant for twin primes)  */
/*  We use a rational approximation     1902160583104 / 10^12   */
/*  The numerator and denominator fit in 64 bits, and n*NUM < 2^64
   for all n up to at least 10^6 (enough for the required range).   */

seq_t a(idx_t n)
{
    /* numerator and denominator of the constant 1.902160583104.  */
    const unsigned long long NUM = 1902160583104ULL;   /* 1.902160583104 × 10^12 */
    const unsigned long long DEN = 1000000000000ULL;   /* 10^12                */

    /* Compute floor(n * NUM / DEN) using 64‑bit unsigned arithmetic.  */
    return (seq_t)((n * NUM) / DEN);
}
