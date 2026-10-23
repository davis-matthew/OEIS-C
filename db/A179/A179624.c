/*
 * A179624  Collatz trajectory starting at 230631.
 * https://oeis.org/A179624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 443, u64 n <= 443, u128 n <= 443,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=230631; for n>1, a(n)= a(n-1)/2 if a(n-1) even else 3*a(n-1)+1 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* values for offset<1 are unused */
    seq_t x = (seq_t)230631;
    for (idx_t i = 1; i < n; ++i) {
        if ((x & 1) == 0)                 /* even */
            x >>= 1;
        else                              /* odd */
            x = x * 3 + 1;
    }
    return x;
}
