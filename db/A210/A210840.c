/*
 * A210840  Sum of the 8th powers of the digits of n.
 * https://oeis.org/A210840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of the eighth powers of the decimal digits of n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Precomputed 8th powers of digits 0-9 */
    static const seq_t POW8[10] = {
        0ULL,           /* 0^8 */
        1ULL,           /* 1^8 */
        256ULL,         /* 2^8 */
        6561ULL,        /* 3^8 */
        65536ULL,       /* 4^8 */
        390625ULL,      /* 5^8 */
        1679616ULL,     /* 6^8 */
        5764801ULL,     /* 7^8 */
        16777216ULL,    /* 8^8 */
        43046721ULL     /* 9^8 */
    };

    seq_t sum = 0;
    idx_t x = n;
    while (x > 0) {
        int d = x % 10;        /* digit in the ones place */
        sum += POW8[d];
        x /= 10;               /* discard the processed digit */
    }
    return sum;
}
