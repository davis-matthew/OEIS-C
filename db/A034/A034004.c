/*
 * A034004  Successive decimal digits of triangular numbers.
 * https://oeis.org/A034004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   successive decimal digits of triangular numbers
 * Range:     u8 n <= 52, u16 n <= 2193, u32 n <= 5355, u64 n <= 5355,
 *            u128 n <= 5355, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t pos = 0;
    for (idx_t k = 0; ; ++k)
    {
        seq_t t = (seq_t)k * ((seq_t)k + 1) / 2;

        /* Extract decimal digits of t into an array (least‑significant first) */
        unsigned char digits[128];
        int di = 0;
        seq_t tmp = t;
        if (tmp == 0)
        {
            digits[di++] = 0;
        }
        else
        {
            while (tmp != 0)
            {
                digits[di++] = (unsigned char)(tmp % 10);
                tmp /= 10;
            }
        }

        /* Emit digits from most‑significant to least‑significant */
        for (int wi = di - 1; wi >= 0; --wi)
        {
            ++pos;
            if (pos == n)
            {
                return (seq_t)digits[wi];
            }
        }
    }
}
