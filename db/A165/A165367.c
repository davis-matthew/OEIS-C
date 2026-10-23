/*
 * A165367  Trisection a(n) = A026741(3n + 2).
 * https://oeis.org/A165367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) - a(n-4) for n >= 4; a(0) = 1, a(1) = 5, a(2) =
 *            4, a(3) = 11
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the first four fixed values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)11;

    /* Use a rolling buffer of the last four computed terms.
       Since the recurrence uses only a(n-2) and a(n-4),
       keeping the values modulo 4 suffices. */
    seq_t buf[4] = { (seq_t)1, (seq_t)5, (seq_t)4, (seq_t)11 };

    for (idx_t i = 4; i <= n; ++i)
    {
        /* Indexing modulo 4 using bitwise AND with 3 is portable
           and fast for powers‑of‑two modulus. */
        idx_t idx  = i       & 3;   /* i      mod 4  */
        idx_t idx2 = (i-2)    & 3; /* i-2    mod 4  */
        idx_t idx4 = (i-4)    & 3; /* i-4    mod 4  */

        buf[idx] = ((seq_t)2 * buf[idx2]) - buf[idx4];
    }

    return buf[n & 3];
}
