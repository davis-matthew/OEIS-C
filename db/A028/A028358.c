/*
 * A028358  Partial sums of A028357.
 * https://oeis.org/A028358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A028358:  Partial sums of A028357.
    Formula: a(n) = (9 + 72*n + 30*n^2 + 32*cos((n + 1)*Pi/3) - (-1)^n) / 24
    cos((n + 1)*Pi/3) takes the values [32,16,-16,-32,-16,16] for (n+1) mod 6 = 0..5.
    (-1)^n is 1 for even n, -1 for odd n.                                       */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for calculations */
    const seq_t N = n;
    /* Compute (n+1) mod 6 to select the cos term */
    idx_t mod6 = (n + 1) % 6;
    /* Cosine term values */
    int cos_val;                /* Fits in int since range is -32..32 */
    switch (mod6) {
        case 0: cos_val = 32; break;
        case 1: cos_val = 16; break;
        case 2: cos_val = -16; break;
        case 3: cos_val = -32; break;
        case 4: cos_val = -16; break;
        case 5: cos_val = 16; break;
    }
    /* (-1)^n */
    int neg_power = (n & 1) ? -1 : 1;   /* 1 if n even, -1 if n odd */

    /* Compute numerator: 9 + 72*n + 30*n^2 + cos_val - neg_power */
    seq_t numerator = 9;               /* constant term */
    numerator += (seq_t)72 * N;        /* 72*n */
    numerator += (seq_t)30 * N * N;    /* 30*n^2 */
    numerator += (seq_t)cos_val;       /* cos term */
    numerator -= (seq_t)neg_power;     /* subtract (-1)^n */

    /* Division by 24 gives the exact integer result */
    return numerator / (seq_t)24;
}
