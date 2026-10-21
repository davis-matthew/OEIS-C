/*
 * A083335  a(n) = 12*a(n-2) - 25*a(n-4) with initial terms 1,1,7,12.
 * https://oeis.org/A083335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-2) - 25*a(n-4) a(0) = 1, a(1) = 1, a(2) = 7, a(3)
 *            = 12
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four values directly */
    seq_t const v0 = 1;
    seq_t const v1 = 1;
    seq_t const v2 = 7;
    seq_t const v3 = 12;

    if (n < 4)
    {
        switch ((unsigned int)n)
        {
            case 0: return v0;
            case 1: return v1;
            case 2: return v2;
            case 3: return v3;
        }
    }

    /* Ring buffer holding a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t ring[4] = {v0, v1, v2, v3};

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t new_val = 12 * ring[2] - 25 * ring[0];

        ring[0] = ring[1];
        ring[1] = ring[2];
        ring[2] = ring[3];
        ring[3] = new_val;
    }

    return ring[3];
}
