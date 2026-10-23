/*
 * A036170  Log base 5 (n) mod 47.
 * https://oeis.org/A036170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = discrete logarithm base 5 of n modulo 47 (i.e. the least
 *            non‑negative k such that 5^k ≡ n (mod 47), a(1)=0)
 * Range:     u8 n <= 46, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Precomputed discrete log table for the residues 0‑46 modulo 47.
       log5[r] gives the exponent k with 5^k ≡ r (mod 47).  For r==0
       the logarithm is undefined; we return 0 by convention. */
    static const seq_t log5[47] = {
        0, /* 0  -> 0 (undefined, convention) */
        0, /* 1  -> 0 */
        18,/* 2  -> 18 */
        20,/* 3  -> 20 */
        36,/* 4  -> 36 */
        1, /* 5  -> 1 */
        38,/* 6  -> 38 */
        32,/* 7  -> 32 */
        8, /* 8  -> 8 */
        40,/* 9  -> 40 */
        19,/*10  -> 19 */
        7, /*11  -> 7 */
        10,/*12  -> 10 */
        11,/*13  -> 11 */
        4, /*14  -> 4 */
        21,/*15  -> 21 */
        26,/*16  -> 26 */
        16,/*17  -> 16 */
        12,/*18  -> 12 */
        45,/*19  -> 45 */
        37,/*20  -> 37 */
        6, /*21  -> 6 */
        25,/*22  -> 25 */
        5, /*23  -> 5 */
        28,/*24  -> 28 */
        2, /*25  -> 2 */
        29,/*26  -> 29 */
        14,/*27  -> 14 */
        22,/*28  -> 22 */
        35,/*29  -> 35 */
        39,/*30  -> 39 */
        3, /*31  -> 3 */
        44,/*32  -> 44 */
        27,/*33  -> 27 */
        34,/*34  -> 34 */
        33,/*35  -> 33 */
        30,/*36  -> 30 */
        42,/*37  -> 42 */
        17,/*38  -> 17 */
        31,/*39  -> 31 */
        9, /*40  -> 9 */
        15,/*41  -> 15 */
        24,/*42  -> 24 */
        13,/*43  -> 13 */
        43,/*44  -> 43 */
        41,/*45  -> 41 */
        23  /*46  -> 23 */
    };

    /* Reduce n modulo 47.  The cast is safe because n is an unsigned idx_t. */
    idx_t r = n % 47;
    if (r == 0) {
        /* n divisible by 47; logarithm undefined.  We return 0 by convention. */
        return 0;
    }
    return log5[r];
}
