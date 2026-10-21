/*
 * A094560  Initial decimal digit of Pi*n, Pi=3.1415...
 * https://oeis.org/A094560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the first decimal digit (most significant digit) of
 *            floor(π · n) * where π = 3.141592653589793238... . * The
 *            implementation uses an integer approximation of π scaled by
 *            10^15, * so that all calculations are performed with 128‑bit
 *            integers and * no floating‑point arithmetic is required
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Scaled value of π: π × 10^15, rounded down */
    static const unsigned long long PI_SCALED = 3141592653589793ULL;
    /* Scaling factor 10^15 */
    static const unsigned long long SCALE = 1000000000000000ULL;

    /* For n == 0 the sequence is undefined; return 0 as a safe default. */
    if (n == 0)
        return (seq_t)0;

    /* Compute floor(π · n) using integer arithmetic */
    __int128 prod = (__int128)PI_SCALED * n;
    unsigned long long integer_part = (unsigned long long)(prod / SCALE);

    /* Extract the most significant digit (leading decimal digit) */
    while (integer_part >= 10)
        integer_part /= 10;

    return (seq_t)integer_part;
}
