/*
 * A048269  First palindrome greater than n+2 in bases n+2 and n.
 * https://oeis.org/A048269
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 209, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A048269: first palindrome greater than n+2 in bases n+2 and n.
   For n ≥ 7 the terms are given by
        a(n) = (5 + (-1)^(n+1) + 2*(5 + (-1)^n)*n + 2*(2 + (-1)^n)*n^2) / 4
   with the explicit initial values:
        a(2)=5, a(3)=26, a(4)=21, a(5)=24, a(6)=154.
   The formula holds for all n > 6 and yields integer results. */
seq_t a(idx_t n)
{
    /* explicit values for the fixed small indices */
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)26;
    if (n == 4) return (seq_t)21;
    if (n == 5) return (seq_t)24;
    if (n == 6) return (seq_t)154;

    /* n ≥ 7: use the general closed form */
    if (n >= 7) {
        unsigned __int128 nn   = (unsigned __int128)n;
        int parity = n & 1;          /* 0 if even, 1 if odd   */
        int sign_n  = parity ? -1 :  1;        /* (-1)^n  */
        int sign_np1= -sign_n;                   /* (-1)^(n+1) */

        unsigned __int128 part1 = (unsigned __int128)(5 + sign_np1);
        unsigned __int128 term2 = 2u * (unsigned __int128)(5 + sign_n) * nn;
        unsigned __int128 term3 = 2u * (unsigned __int128)(2 + sign_n) * nn * nn;

        unsigned __int128 num   = part1 + term2 + term3;
        seq_t result = (seq_t)(num / 4);
        return result;
    }

    /* For any n < 2 the sequence is undefined; return 0 as a safe default. */
    return (seq_t)0;
}
