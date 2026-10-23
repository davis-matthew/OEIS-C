/*
 * A107684  Union of sequences 2^k-1, 2^k and 2^k+1.
 * https://oeis.org/A107684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = if n <= 2 then n else 2^(floor(n/3)+1) + n mod 3 - 1
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)                /* n = 0,1,2 */
        return (seq_t)n;

    /* exponent = floor(n/3) + 1 */
    unsigned int exp = (unsigned int)(n / 3 + 1);

    /* 2^exp computed as shift; for big‑int types this is defined */
    seq_t val = ((seq_t)1 << exp);

    /* add (n mod 3) and subtract 1 */
    val += (seq_t)(n % 3);
    val -= (seq_t)1;

    return val;
}
