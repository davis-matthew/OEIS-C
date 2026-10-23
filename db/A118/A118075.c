/*
 * A118075  Start with 1 and repeatedly reverse the digits and add 42 to get
 *          the next term.
 * https://oeis.org/A118075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 37, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;
   for n > 1: a(n) = reverse_digits(a(n-1)) + 42. */

static inline seq_t rev_digits(seq_t v)
{
    seq_t rev = (seq_t)0;
    while(v != (seq_t)0) {
        rev = rev * (seq_t)10 + v % (seq_t)10;
        v   = v / (seq_t)10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    if(n < (idx_t)1) return (seq_t)0;   /* undefined for n < 1 */

    seq_t cur = (seq_t)1;               /* a(1) */
    idx_t i   = (idx_t)1;

    while(i < n) {
        cur = rev_digits(cur) + (seq_t)42;
        ++i;
    }
    return cur;
}
