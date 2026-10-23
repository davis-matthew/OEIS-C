/*
 * A020337  Numbers whose base-9 representation is the juxtaposition of two
 *          identical strings.
 * https://oeis.org/A020337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n*9^floor(log_9(n)+1) + n
 * Range:     u8 n <= 8, u16 n <= 89, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of base‑9 digits of n, raise 9 to that power,
   and return n*(9^d)+n. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* not part of OEIS offset, but safe */

    /* number of digits of n in base 9 */
    idx_t t = n;
    idx_t d = 0;
    while (t > 0) {     /* t always positive when n >= 1 */
        t /= 9;
        d++;
    }

    /* compute 9^d as seq_t */
    seq_t pow9 = 1;
    for (idx_t i = 0; i < d; ++i) {
        pow9 *= (seq_t)9;
    }

    /* a(n) = n * pow9 + n */
    seq_t res = (seq_t)n * pow9 + (seq_t)n;
    return res;
}
