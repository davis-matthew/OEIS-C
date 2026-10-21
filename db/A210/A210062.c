/*
 * A210062  Number of digits in 7^n.
 * https://oeis.org/A210062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(n*log10(7)) + 1
 * Range:     u8 n <= 301, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a high‑precision long double constant for log10(7) to avoid floating point
   arithmetic in the result. */

static const long double LOG10_7 = 0.845098040014256830L;

seq_t a(idx_t n)
{
    /* compute n*log10(7) with long double */
    long double val = (long double)n * LOG10_7;
    /* floor by truncating to unsigned long long, then add 1 */
    unsigned long long digits = (unsigned long long)val + 1ULL;
    return (seq_t)digits;
}
