/*
 * A103481  Diagonal sums of A103462.
 * https://oeis.org/A103481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0..floor(n/2)} k^(n-2k) + floor((n+1)/2)
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t limit = n / 2;          /* floor(n/2) */
    seq_t total = 0;

    for (idx_t k = 0; k <= limit; ++k)
    {
        idx_t exp = n - 2 * k;    /* exponent n-2k */
        seq_t base = (seq_t)k;
        seq_t res = 1;
        idx_t e = exp;
        seq_t b = base;

        /* binary exponentiation (modulo arithmetic)
           works for 0^0 == 1 automatically */
        while (e > 0)
        {
            if (e & 1ULL)
                res *= b;
            e >>= 1ULL;
            if (e)
                b *= b;
        }

        total += res;
    }

    /* add the floor((n+1)/2) term */
    total += ((seq_t)(n + 1)) / 2;

    return total;
}
