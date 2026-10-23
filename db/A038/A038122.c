/*
 * A038122  Start with {1,2,...,n}, replace any two numbers a,b with
 *          |a^2-b^2|, repeat until single number k remains; a(n) = minimal
 *          value of k.
 * https://oeis.org/A038122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n*(n+1)/2 mod 6 for n<4 or n>7; special cases are
 *            a(4)=16, a(5)=15, a(6)=63, a(7)=8
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special cases for n = 1..7 */
    if (n <= 7) {
        switch ((idx_t)n) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)3;
            case 3: return (seq_t)0;
            case 4: return (seq_t)16;
            case 5: return (seq_t)15;
            case 6: return (seq_t)63;
            case 7: return (seq_t)8;
        }
    }

    /* For n > 7 compute (n*(n+1)/2) mod 6 without overflow.
       Since n*(n+1) is even, we can reduce modulo 12, then divide by 2. */
    idx_t n_mod12  = n % (idx_t)12;
    idx_t n1_mod12 = (n + (idx_t)1) % (idx_t)12;
    idx_t prod_mod12 = (n_mod12 * n1_mod12) % (idx_t)12;
    seq_t result = (seq_t)(prod_mod12 / (idx_t)2);
    return result;
}
