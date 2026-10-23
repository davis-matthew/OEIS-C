/*
 * A036668  Hati numbers: of form 2^i*3^j*k, i+j even, (k,6)=1.
 * https://oeis.org/A036668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 146, u16 n <= 9458, u32 n <= 9458, u64 n <= 9458,
 *            u128 n <= 9458, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers of the form 2^i * 3^j * k  with (i + j) even
 * and (k, 6) = 1.  The function returns the n‑th such number
 * (a(1) = 1).  The algorithm tests consecutive integers until
 * the desired count is reached.  It works for any integer type
 * seq_t (primitive or big integer) and any idx_t (uint64_t). */

seq_t a(idx_t n)
{
    if (n == 0)   /* defined behaviour for n < 1 */
        return 0;

    seq_t m = 1;          /* current integer tested */
    idx_t count = 0;      /* how many Hati numbers seen so far */

    while (1) {
        seq_t t = m;
        unsigned e2 = 0, e3 = 0;

        /* Count factors of 2 */
        while (t % (seq_t)2 == 0) {
            t /= (seq_t)2;
            ++e2;
        }
        /* Count factors of 3 */
        while (t % (seq_t)3 == 0) {
            t /= (seq_t)3;
            ++e3;
        }

        /* Accept if the sum of the exponents is even */
        if (((e2 + e3) & 1) == 0) {
            ++count;
            if (count == n) {
                return m;
            }
        }
        ++m;   /* next integer to test */
    }
}
