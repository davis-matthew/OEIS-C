/*
 * A029922  Convert n from kilometers (km) to miles.
 * https://oeis.org/A029922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/1.609344 + 1/2)
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* using fraction 15625/25146 for exact integer rounding */
seq_t a(idx_t n)
{
    /* 1 mile = 1.609344 km  => 1 km = 15625/25146 miles */
    /* nearest integer:  floor(n * 15625/25146 + 1/2)  */
    const seq_t NUM  = 15625;   /* numerator of miles per km fraction */
    const seq_t DEN  = 25146;   /* denominator of miles per km fraction */
    const seq_t HALF = 12573;   /* HALF = DEN/2 for rounding */

    seq_t num = ((seq_t)n) * NUM + HALF;  /* numerator after adding half denominator */
    return num / DEN;                     /* integer division gives floor(ratio + 1/2) */
}
