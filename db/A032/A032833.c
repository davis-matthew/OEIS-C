/*
 * A032833  Numbers whose set of base-9 digits is {3,4}.
 * https://oeis.org/A032833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 62, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑9 digits are 3 or 4:
   a(n) is obtained by writing (n‑1) in binary, mapping 0→3 and 1→4,
   then interpreting the resulting digits as a base‑9 number. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; behaviour for n==0 is irrelevant */

    /* Find the length L of the base‑9 number.  All numbers of length < L
       account for 2 + 4 + 8 + … + 2^L‑1 = 2^(L+1)‑2 terms.           */
    idx_t remaining = n;
    idx_t len = 1;
    while (remaining > ((idx_t)1 << len)) {
        remaining -= ((idx_t)1 << len);
        ++len;
    }

    /* remaining is now the 1‑based index inside the group of length 'len'.
       Convert it to 0‑based offset.                                             */
    idx_t offset = remaining - 1;

    /* Build the number by mapping binary bits to digits 3 (0) / 4 (1).      */
    seq_t value = 0;
    for (idx_t i = len; i-- > 0; ) {
        seq_t digit = ((offset >> i) & 1) ? (seq_t)4 : (seq_t)3;
        value = value * (seq_t)9 + digit;
    }
    return value;
}
