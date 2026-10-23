/*
 * A163329  Inverse permutation to A163328.
 * https://oeis.org/A163329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001477bi(A163325(n),A163326(n)) = ((x + y)^2 + x +
 *            3*y)/2 where (x, y) are obtained by taking the base‑3 digits of
 *            n and splitting them into even and odd positions (0‑based)
 * Range:     u8 n <= 133, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: uses only seq_t and idx_t types supplied by the build. */

seq_t a(idx_t n)
{
    seq_t x = 0;
    seq_t y = 0;
    seq_t pow_x = 1;   /* 3^k for the next even digit of x      */
    seq_t pow_y = 1;   /* 3^k for the next odd digit of y       */
    idx_t  i   = 0;    /* digit position (0‑based)              */
    idx_t  m   = n;    /* working copy of n                    */

    /* Decompose n into base‑3 digits, assigning even positions
       to x and odd positions to y. */
    while (m > 0) {
        idx_t d = m % 3;          /* current base‑3 digit (0,1,2) */
        m /= 3;

        if ((i & 1) == 0) {      /* even position → x */
            x += (seq_t)d * pow_x;
            pow_x *= 3;
        } else {                  /* odd position  → y */
            y += (seq_t)d * pow_y;
            pow_y *= 3;
        }
        ++i;
    }

    /* Cantor pairing: a = ((x + y)^2 + x + 3*y)/2 */
    seq_t s   = x + y;
    seq_t res = (s * (s + 1)) / 2 + y;
    return res;
}
