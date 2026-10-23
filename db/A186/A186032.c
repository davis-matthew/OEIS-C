/*
 * A186032  a(n) = (-1)^A048881(n).
 * https://oeis.org/A186032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^A048881(n)
 * Range:     i8 n <= 60, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where A048881(n) = the exponent of 2 in the reduced fraction
 *          C(n)/2^n  (C(n) is the nth Catalan number).
 *  Using v2(C_n) = popcount(n) - v2(n+1),
 *  a(n) = (-1)^{v2(C_n)} for n>0 (and also for n=0).
 *  Hence a(n) = +1 if v2(C_n) is even, -1 if it is odd.  */

static inline unsigned int popcount64(uint64_t x)
{
    unsigned int c = 0;
    while (x) {
        x &= x - 1;   /* clear lowest set bit */
        c++;
    }
    return c;
}

static inline int v2_uint64(uint64_t x)
{
    if (x == 0) return 64;  /* undefined; never occurs for n+1 > 0 in tests */
    int c = 0;
    while ((x & 1) == 0) {
        x >>= 1;
        c++;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* Compute v2(C_n) = popcount(n) - v2(n+1) */
    unsigned int pop  = popcount64(n);
    unsigned int v2_n = v2_uint64(n + 1);

    int v2C = (int)pop - (int)v2_n;   /* v2C >= 0 for all n >= 0 */

    /* a(n) = (-1)^v2C, i.e. +1 if v2C even, -1 if odd */
    return (v2C & 1) ? (seq_t)-1 : (seq_t)1;
}
