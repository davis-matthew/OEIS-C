/*
 * A043721  Numbers whose number of runs in their base-2 representation is
 *          congruent to 0 mod 3.
 * https://oeis.org/A043721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer whose binary representation *
 *            has a number of runs (maximal blocks of equal bits) that is
 *            congruent * to 0 modulo 3
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A043721.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for n = 0, return 0 */

    idx_t found = 0;                /* how many terms found so far */
    idx_t x = 1;                    /* candidate integer to test */

    while (found < n) {
        /* Count the number of runs in the binary representation of x. */
        idx_t runs = 1;              /* at least one run */
        idx_t prev = x & 1;          /* least significant bit */
        idx_t tmp = x >> 1;          /* remaining bits */

        while (tmp) {
            idx_t bit = tmp & 1;
            if (bit != prev) {
                runs++;
                prev = bit;
            }
            tmp >>= 1;
        }

        if ((runs % 3) == 0) {       /* accepted number */
            found++;
            if (found == n)
                return (seq_t)x;
        }
        x++;                          /* try next integer */
    }
    return (seq_t)0;                 /* unreachable, keeps compiler happy */
}
