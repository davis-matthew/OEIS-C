/*
 * A163529  The Y-coordinate of the n-th point in the Peano curve A163334.
 * https://oeis.org/A163529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 8921, u16 n <= 8921, u32 n <= 8921, u64 n <= 8921,
 *            u128 n <= 8921, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Alg: a(n) is the Y‑coordinate of the n‑th point of the Peano curve.
   n is taken in base 9, the Y‑digit in base 3 is built by a 2‑state
   automaton.
   For each base‑9 digit d (most significant first) we compute
        k   = d / 3          // 0,1,2
        y   = y * 3 + (state ? (2 - k) : k)
   and after processing the digit we toggle the state if d % 3 == 1
   (i.e., for digits 1, 4, or 7).  This reproduces the OEIS sequence
   A163529 for all n >= 0.
*/
seq_t a(idx_t n)
{
    seq_t y = 0;
    int state = 0;            /* 0 = normal, 1 = reversed   */
    idx_t pow = 1;

    /* Find the highest power of 9 <= n (safe from overflow). */
    while (pow <= n / 9) {
        pow *= 9;
    }

    /* Process each base‑9 digit, from most significant to least. */
    while (pow > 0) {
        idx_t d = n / pow;        /* current base‑9 digit (0…8) */
        n -= d * pow;             /* remainder for the next digit */

        int k   = (int)(d / 3);   /* digit’s quotient by 3 (0…2) */
        int out = state ? (2 - k) : k;

        y = y * 3 + out;

        if (d % 3 == 1) {         /* toggle state on digits 1,4,7 */
            state ^= 1;
        }

        pow /= 9;
    }
    return y;
}
