/*
 * A236840  n minus number of runs in the binary expansion of n: a(n) = n -
 *          A005811(n).
 * https://oeis.org/A236840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n - A005811(n) (number of runs in binary expansion of n)
 * Range:     u8 n <= 258, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* a(0) = 0 by definition */

    /* Count transitions between consecutive bits of n */
    idx_t rot = n;                     /* copy so we can shift */
    idx_t prev_bit = rot & 1;
    rot >>= 1;
    idx_t trans = 0;                   /* number of bit transitions */

    while (rot) {
        idx_t cur_bit = rot & 1;
        if (cur_bit != prev_bit)
            trans++;
        prev_bit = cur_bit;
        rot >>= 1;
    }

    /* a(n) = n - (number of runs) = n - (transitions + 1) */
    return (seq_t)n - ((seq_t)trans + 1);
}
