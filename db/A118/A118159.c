/*
 * A118159  Start with 1 and repeatedly reverse the digits and add 64 to get
 *          the next term.
 * https://oeis.org/A118159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less, easy
 * Formula:   a(1)=1; a(n)=reverse_of_decimal(a(n-1)) + 64 for n>1
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the decimal reverse of a seq_t value.  This avoids any external
   helper and works for all supported seq_t types (uint8_t..unsigned __int128
   or arbitrary‑precision).  Only allowed operators are used. */
static inline seq_t reverse_decimal(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        rev = rev * 10 + (x % 10);
        x   = x / 10;
    }
    return rev;
}

/* Return the n‑th term of the OEIS sequence A118159.  The sequence is
   defined for n >= 1; behaviour for n < 1 is irrelevant. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;     /* below offset: arbitrary, allowed */

    seq_t val = 1;           /* a(1) */
    for (idx_t i = 1; i < n; ++i) {
        val = reverse_decimal(val) + (seq_t)64;
    }
    return val;
}
