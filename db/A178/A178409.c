/*
 * A178409  Number of ways to place 6 nonattacking wazirs on an n X n board.
 * https://oeis.org/A178409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/720 * (n^12 -75*n^10 +60*n^9 +2365*n^8 -3720*n^7
 *            -38085*n^6 * +89580*n^5 +292834*n^4 -984960*n^3 -552240*n^2 *
 *            +4128960*n -3160800) (n ≥ 5)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 11, u64 n <= 69, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n < 5 the sequence is 0,0,0,114 as listed in the OEIS entry. */

seq_t a(idx_t n)
{
    /* Handle the small indices explicitly. */
    if (n < 4) return (seq_t)0;
    if (n == 4) return (seq_t)114;

    /* Use unsigned __int128 for intermediate calculations.  For the
       arbitrary‑precision build seq_t may be a big‑int class that can be
       constructed from an integer constant or an unsigned long long,
       which is where the intermediate __int128 values are cast. */
    unsigned __int128 nn = (unsigned __int128)n;

    /* Compute n^k for k up to 12.  The intermediate powers fit into
       128 bits for all n where the final result fits into seq_t
       (e.g. for 64‑bit seq_t the function is only called for n ≤ ~45). */
    unsigned __int128 pow2  = nn * nn;          /* n^2   */
    unsigned __int128 pow3  = pow2 * nn;        /* n^3   */
    unsigned __int128 pow4  = pow3 * nn;        /* n^4   */
    unsigned __int128 pow5  = pow4 * nn;        /* n^5   */
    unsigned __int128 pow6  = pow5 * nn;        /* n^6   */
    unsigned __int128 pow7  = pow6 * nn;        /* n^7   */
    unsigned __int128 pow8  = pow7 * nn;        /* n^8   */
    unsigned __int128 pow9  = pow8 * nn;        /* n^9   */
    unsigned __int128 pow10 = pow9 * nn;        /* n^10  */
    unsigned __int128 pow11 = pow10 * nn;       /* n^11  */
    unsigned __int128 pow12 = pow11 * nn;       /* n^12  */

    /* Assemble the numerator of the formula.  All sub‑expressions are
       performed in 128‑bit arithmetic to avoid intermediate overflow
       for the values of interest. */
    unsigned __int128 num = 0;

    num += pow12;
    num -= 75u * pow10;
    num += 60u * pow9;
    num += 2365u * pow8;
    num -= 3720u * pow7;
    num -= 38085u * pow6;
    num += 89580u * pow5;
    num += 292834u * pow4;
    num -= 984960u * pow3;
    num -= 552240u * pow2;
    num += 4128960u * nn;
    num -= 3160800u;

    /* Divide by 720 (6!) – the numerator is always divisible by 720
       for integer n ≥ 5.  The division is exact and the result
       fits in seq_t for all n where the sequence value does. */
    unsigned __int128 result128 = num / 720u;

    /* Cast to the user‑supplied seq_t type.  On a 64‑bit build result128
       is guaranteed to fit into a uint64_t; on an arbitrary‑precision
       build the cast yields a big integer. */
    return (seq_t)result128;
}
