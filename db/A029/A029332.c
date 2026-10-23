/*
 * A029332  Expansion of 1/((1-x^4)*(1-x^5)*(1-x^7)*(1-x^8)).
 * https://oeis.org/A029332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3+36*n^2+236*n+2112)/6720 * + n*((n mod
 *            4)-2)^2/64 * + ((n^3+n^2+n+2) mod 5)/5 * + ((6*n^3+6*n^2+2*n+2)
 *            mod 7)/7)
 * Range:     u8 n <= 1, u16 n <= 27, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to a seq_t once. */
    seq_t ns = (seq_t)n;

    /* First rational term: (n^3+36*n^2+236*n+2112)/6720. */
    seq_t term1 = ns * ns * ns
              + (seq_t)36 * ns * ns
              + (seq_t)236 * ns
              + (seq_t)2112;

    /* Second term:  n*((n mod 4)-2)^2/64  =>  (105 * n * ((n mod 4)-2)^2)/6720. */
    uint8_t m4 = (uint8_t)(n % 4);
    int v = (int)m4 - 2;            /* v ∈ {‑2,‑1,0,1} */
    int v2 = v * v;                 /* v^2 ∈ {0,1,4} */
    seq_t term2 = (seq_t)105 * ns * (seq_t)v2;

    /* Modular part of size 5: ((n^3+n^2+n+2) mod 5)/5. */
    seq_t mod5 = (ns * ns * ns
                + ns * ns
                + ns
                + (seq_t)2) % (seq_t)5;
    seq_t term3 = (seq_t)1344 * mod5;

    /* Modular part of size 7: ((6*n^3+6*n^2+2*n+2) mod 7)/7. */
    seq_t mod7 = ((seq_t)6 * ns * ns * ns
                + (seq_t)6 * ns * ns
                + (seq_t)2 * ns
                + (seq_t)2) % (seq_t)7;
    seq_t term4 = (seq_t)960 * mod7;

    /* Combine all numerators (common denominator 6720). */
    seq_t numerator = term1 + term2 + term3 + term4;

    /* Final answer is floor(numerator / 6720). */
    seq_t result = numerator / (seq_t)6720;
    return result;
}
