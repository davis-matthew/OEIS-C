/*
 * A200436  Number of -n..n arrays x(0..8) of 9 elements with zero sum and no
 *          two or three adjacent elements summing to zero.
 * https://oeis.org/A200436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (259723/2240)*n^8 - (2162653/5040)*n^7 +
 *            (1479629/1440)*n^6 * - (642659/360)*n^5 + (6107509/2880)*n^4 -
 *            (1200571/720)*n^3 * + (3930541/5040)*n^2 - (22719/140)*n
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) /* offset start at 1; undefined for n=0, return 0 */
        return 0;

    seq_t N = (seq_t)n;

    /* Compute powers of n (n^1 .. n^8) */
    seq_t power[9];
    seq_t p = 1;
    for (int e = 1; e <= 8; ++e) {
        p *= N;
        power[e] = p;
    }

    /* Coefficients for n^8 down to n^1, signed */
    const int coeffs[8] = {
        2337507,   /* + (259723/2240) */
        -8650612,  /* - (2162653/5040) */
        20714806,  /* + (1479629/1440) */
        -35988904, /* - (642659/360) */
        42752563,  /* + (6107509/2880) */
        -33615988, /* - (1200571/720) */
        15722164,  /* + (3930541/5040) */
        -3271536   /* - (22719/140)  */
    };

    seq_t pos = 0; /* sum of positive contributions */
    seq_t neg = 0; /* sum of negative contributions */

    for (int i = 0; i < 8; ++i) {
        int exp = 8 - i;                /* exponent corresponding to coeffs[i] */
        seq_t term = power[exp] *
                     (seq_t)(coeffs[i] > 0 ? coeffs[i] : -coeffs[i]);

        if (coeffs[i] > 0)
            pos += term;
        else
            neg += term;
    }

    /* pos >= neg for all n >= 1; compute the integer result */
    seq_t diff = pos - neg;
    return diff / 20160;
}
