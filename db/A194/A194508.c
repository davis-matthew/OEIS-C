/*
 * A194508  First coordinate of the (2,3)-Lagrange pair for n.
 * https://oeis.org/A194508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*n - 3*floor((3*n+2)/5)
 * Range:     i8 n <= 633, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* AKA: a(n+5) = a(n) + 1 with base pattern
   n mod 5: 1→-1, 2→1, 3→0, 4→2, 0→1  (where 0 denotes n≡0 mod5). */

seq_t a(idx_t n)
{
    /* number of complete periods of length 5 */
    idx_t k = n / 5;
    /* position within the current period (0..4) */
    idx_t r = n % 5;
    seq_t res;

    switch (r) {
        case 0: /* n ≡ 0 (mod 5) */
            res = (seq_t)k;          /* a = k */
            break;
        case 1: /* n ≡ 1 (mod 5) */
            res = (seq_t)k - 1;      /* a = k – 1 */
            break;
        case 2: /* n ≡ 2 (mod 5) */
            res = (seq_t)k + 1;      /* a = k + 1 */
            break;
        case 3: /* n ≡ 3 (mod 5) */
            res = (seq_t)k;          /* a = k */
            break;
        case 4: /* n ≡ 4 (mod 5) */
            res = (seq_t)k + 2;      /* a = k + 2 */
            break;
        default:
            res = 0;                 /* unreachable, but keeps compiler happy */
    }

    return res;
}
