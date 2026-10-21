/*
 * A236629  Decimal expansion of G/(3*c^2), where G is the Newtonian constant
 *          of gravitation and c is the speed of light in vacuum, in SI units.
 * https://oeis.org/A236629
 *
 * Offset:    -27    (a(-27) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the (n+55)-th decimal digit of G/(3*c^2) Sequence A236629
 *            – decimal expansion of G/(3*c^2), offset -27
 * Range:     u8 n <= -24, u16 n <= -24, u32 n <= -24, u64 n <= -24,
 *            u128 n <= -24, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n < -27 the term is undefined – return 0 */

seq_t a(idx_t n)
{
    /* no work for indices below the defined offset */
    if (n < -27) return 0;

    /* digit position to generate (after the decimal point) */
    idx_t k = n + 55;          /* k ≥ 28 for n ≥ -27 */

    /* constants for the fraction G/(3*c^2)
       G = 6.67430e-11,  c = 299792458 m/s
       G/(3*c²) = 66743 / (269626553621045292 * 10¹⁵) */
    const unsigned __int128 N     = (unsigned __int128)66743;           /* numerator  */
    const unsigned __int128 DEN1  = (unsigned __int128)269626553621045292ULL; /* part of denominator */
    const unsigned __int128 POW10_15 = (unsigned __int128)1000000000000000ULL; /* 10¹⁵         */
    const unsigned __int128 D     = DEN1 * POW10_15;   /* full denominator */

    unsigned __int128 r = N;            /* current remainder   */
    unsigned __int128 digit = 0;        /* digit being computed */

    for (idx_t i = 0; i < k; ++i)      /* generate k decimal digits */
    {
        r *= 10;                      /* bring next decimal place */
        digit = r / D;                /* next decimal digit   */
        r     = r % D;                /* remainder for next step */
    }

    return (seq_t)digit;              /* final digit is 0‑9, fits in seq_t */
}
