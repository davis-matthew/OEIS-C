/*
 * A226578  Smallest number of integer-sided squares needed to tile a 5 X n
 *          rectangle.
 * https://oeis.org/A226578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = base[n] for n ≤ 6; a(n) = base[n - 5*k] + k, where k =
 *            ceil((n - 6)/5) (for n > 6)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t _base[7] = { (seq_t)0, (seq_t)5, (seq_t)4, (seq_t)4,
                                (seq_t)5, (seq_t)1, (seq_t)5 };

seq_t a(idx_t n)
{
    if (n <= 6)
        return _base[n];

    idx_t diff = n - 6;
    idx_t k    = (diff + 4) / 5;            /* ceil((n-6)/5)            */
    idx_t r    = n - 5 * k;                 /* r ≤ 6                      */
    return _base[r] + (seq_t)k;
}
