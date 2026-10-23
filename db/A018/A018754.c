/*
 * A018754  Divisors of 976.
 * https://oeis.org/A018754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 976 in ascending order.  
   a(1)=1, a(2)=2, a(3)=4, a(4)=8, a(5)=16,
   a(6)=61, a(7)=122, a(8)=244, a(9)=488, a(10)=976. */

seq_t a(idx_t n)
{
    if (n <= 5)
        return ((seq_t)1) << (n - 1);
    else if (n <= 10)
        return ((seq_t)61) << (n - 6);
    else
        return (seq_t)0;
}
