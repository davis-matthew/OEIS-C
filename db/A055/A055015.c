/*
 * A055015  Sum of 6th powers of digits of n.
 * https://oeis.org/A055015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = Sum_{d digit of n} d^6 (digits in base 10)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 999, u64 n <= 999, u128 n <= 999,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑computed sixth powers of 0–9 */
    static const seq_t pow6[10] = {
        0ULL,                  /* 0^6 */
        1ULL,                  /* 1^6 */
        64ULL,                 /* 2^6 */
        729ULL,                /* 3^6 */
        4096ULL,               /* 4^6 */
        15625ULL,              /* 5^6 */
        46656ULL,              /* 6^6 */
        117649ULL,             /* 7^6 */
        262144ULL,             /* 8^6 */
        531441ULL              /* 9^6 */
    };

    seq_t sum = 0;          /* result */
    /* Handle n = 0 explicitly (sum stays 0) */
    while (n > 0) {
        idx_t digit = n % 10;   /* last decimal digit */
        sum += pow6[digit];     /* add its sixth power */
        n /= 10;                /* drop the last digit */
    }
    return sum;
}
