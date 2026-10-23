/*
 * A026562  Expansion of 1/((1-2*x)*(1-6*x)*(1-8*x)*(1-10*x)).
 * https://oeis.org/A026562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -2^n/24 + 27*6^n/4 - 64*8^n/3 + 125*10^n/8
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 7, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t power  = base;
    while (exp > 0) {
        if (exp & 1U) result *= power;
        exp >>= 1U;
        if (exp) power *= power;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Compute the four necessary powers. */
    seq_t pow10 = pow_seq((seq_t)10, n);
    seq_t pow8  = pow_seq((seq_t)8,  n);
    seq_t pow6  = pow_seq((seq_t)6,  n);
    seq_t pow2  = pow_seq((seq_t)2,  n);

    /* Assemble numerator: -2^n + 162*6^n - 512*8^n + 375*10^n. */
    seq_t numer = 0;
    numer += 375 * pow10;  /* 375*10^n  (=125*10^n*3) */
    numer += 162 * pow6;   /* 162*6^n   (=27*6^n*6)  */
    numer -= 512 * pow8;   /* -512*8^n  (= -64*8^n*8) */
    numer -= pow2;         /* -2^n      */

    /* Divide by the common denominator 24. */
    return numer / (seq_t)24;
}
