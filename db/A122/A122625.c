/*
 * A122625  a(n) = n_{n^2}.
 * https://oeis.org/A122625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal digits of n interpreted in base n^2
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A122625: a(n) = n_{n^2})                                 */
seq_t a(idx_t n)
{
    /* n^2 as the base of the reinterpretation.  Cast to seq_t
       before multiplication to avoid overflow in the intermediate (idx_t). */
    seq_t base = (seq_t)n * (seq_t)n;
    seq_t result = 0;

    /* Extract decimal digits of n into an array (least‑significant first). */
    idx_t tmp = n;
    unsigned digits[20];           /* 20 digits are enough for a uint64_t */
    int len = 0;
    do
    {
        digits[len++] = (unsigned)(tmp % 10);
        tmp /= 10;
    } while (tmp > 0);

    /* Process digits from most‑significant to least‑significant using Horner. */
    for (int i = len - 1; i >= 0; --i)
    {
        result = result * base + (seq_t)digits[i];
    }

    return result;
}
