/*
 * A082577  a(n) = first digit to the right of decimal point of
 *          n*(sqrt(5)-1)/2.
 * https://oeis.org/A082577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = first decimal digit of n*(√5−1)/2 (i.e. the first digit
 *            to the right of the decimal point of n*(√5−1)/2). This
 *            implementation uses an integer approximation of (√5−1)/2 with 19
 *            decimal‑digit precision and evaluates floor(10*n*(√5−1)/2)
 *            modulo 10 without any floating‑point arithmetic. The ap...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 0 is out of range; return 0 in that case. */
    if (n == 0)
        return (seq_t)0;

    /*  (√5−1)/2 ≈ 0.6180339887498948482
        We store it as NUM/DEN with 19 decimal‑digit precision.
        NUM = 0.6180339887498948482 * 10^19 = 6180339887498948482
        DEN = 10^19 = 10000000000000000000. */
    const unsigned __int128 NUM = 6180339887498948482ULL;   /* α * 10^19 */
    const unsigned __int128 DEN = 10000000000000000000ULL;  /* 10^19     */

    /* Compute floor(10*n*α) as ⌊10*n*NUM/DEN⌋. */
    unsigned __int128 prod = (unsigned __int128)n * 10 * NUM;
    unsigned __int128 val  = prod / DEN;  /* floor(10*n*α) */

    /* The desired digit is the last decimal digit of that floor value. */
    return (seq_t)(val % 10);
}
