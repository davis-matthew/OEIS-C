/*
 * A039603  Numbers whose base-12 representation has the same nonzero number
 *          of 0's and 11's.
 * https://oeis.org/A039603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search: enumerate integers and count base‑12 digits 0 and 11 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* offset undefined for n < 1 */
    idx_t found = 0;
    const seq_t base12 = (seq_t)12;
    seq_t x = (seq_t)1;

    for (;; ++x)
    {
        seq_t tmp = x;
        unsigned int c0  = 0;
        unsigned int c11 = 0;

        while (tmp > (seq_t)0)
        {
            seq_t d = tmp % base12;
            if (d == (seq_t)0)  ++c0;
            else if (d == (seq_t)11) ++c11;
            tmp /= base12;
        }

        if (c0 > 0 && c0 == c11)
        {
            ++found;
            if (found == n) return x;
        }
    }
}
