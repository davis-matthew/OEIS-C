/*
 * A227952  Wow! Signal intensity signature (6EQUJ5).
 * https://oeis.org/A227952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, easy, nonn
 * Formula:   a(n) = the n‑th base‑36 digit of the constant 6EQUJ5 (0 for n >
 *            6)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const char *sig = "6EQUJ5";
    const idx_t len = 6;
    idx_t pos = n - 1;                     /* 0‑based index */
    if (pos >= len)                        /* out of range */
        return (seq_t)0;
    unsigned char c = sig[pos];
    if (c >= '0' && c <= '9')
        return (seq_t)(c - '0');
    else
        return (seq_t)(c - 'A' + 10);
}
