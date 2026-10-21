/*
 * A214584  Integers whose decimal representation has only digits in {4,5,7}.
 * https://oeis.org/A214584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 282, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function generates A214584: numbers whose decimal digits are only 4, 5 or 7. */
/* The sequence is obtained by interpreting the index n (1‑based) as a number in
   base‑3 where the digits 0,1,2 map to 4,5,7 respectively. */
seq_t a(idx_t n)
{
    /* Find how many digits the n‑th number has. */
    idx_t pow3 = 3;          /* 3^1 */
    idx_t cum  = 3;          /* Sum of 3^1 .. 3^d */
    unsigned int d = 1;      /* current digit count */

    while (n > cum) {
        pow3 *= 3;           /* next power of 3 */
        cum  += pow3;        /* update cumulative count */
        d++;
    }

    /* Numbers having fewer than d digits: cum - pow3 */
    idx_t prev = cum - pow3;
    idx_t m    = n - prev - 1;   /* zero‑based offset within the d‑digit block */

    /* Convert m to a base‑3 representation with exactly d digits. */
    uint8_t digits[64] = {0};    /* safe upper bound for d */
    for (int i = (int)d - 1; i >= 0; i--) {
        digits[i] = m % 3;
        m /= 3;
    }

    /* Map base‑3 digits 0→4, 1→5, 2→7 to build the decimal number. */
    static const unsigned char map[3] = {4, 5, 7};
    seq_t val = 0;
    for (unsigned int i = 0; i < d; i++) {
        val = val * 10 + map[digits[i]];
    }
    return val;
}
