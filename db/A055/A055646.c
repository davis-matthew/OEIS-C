/*
 * A055646  Integers in base 15 with each base 15 digit represented by 2
 *          decimal digits.
 * https://oeis.org/A055646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Σ [ (n mod 15) * 10^(2*pos) ] over all base‑15 digits of
 *            n, i.e. write the base‑15 digits of n each as two decimal digits
 *            and interpret the concatenation as a decimal integer
 * Range:     u8 n <= 44, u16 n <= 1574, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;     /* accumulator for the output number      */
    seq_t power10 = 1;    /* 10^(2*pos), starts with 10^0 = 1     */

    while (n > 0)
    {
        seq_t d = n % 15;      /* least significant base‑15 digit   */
        result += d * power10; /* append the 2‑digit decimal value   */
        power10 *= 100;        /* move to next two decimal places   */
        n /= 15;               /* next base‑15 digit                */
    }
    return result;
}
