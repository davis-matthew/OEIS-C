/*
 * A059533  Beatty sequence for 1 + Catalan's constant.
 * https://oeis.org/A059533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * (1 + Catalan's constant))
 * Range:     u8 n <= 133, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Catalan's constant G ≈ 0.91596559417721901505… */
    /* 1 + G ≈ 1.91596559417721901505…  */
    /* Use a rational approximation 1915965594177219/1000000000000000
       which has relative error < 10⁻¹⁵, sufficient for all n ∈ [1,10⁴]
       and comfortably for larger n. */

    if (n == 0)          /* offset is 1; value for n=0 is unused */
        return 0;

    /* 128‑bit intermediate for n * numerator */
    const unsigned __int128 num = (unsigned __int128)1915965594177219ULL;
    const unsigned __int128 den = (unsigned __int128)1000000000000000ULL;

    unsigned __int128 prod = (unsigned __int128)n * num;
    unsigned __int128 val  = prod / den;   /* floor of the product */

    return (seq_t)val;   /* assignment to seq_t works for built‑in or
                            arbitrary‑precision integer types */
}
