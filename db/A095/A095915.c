/*
 * A095915  Each number is twice times the product of the digits of the
 *          previous number.
 * https://oeis.org/A095915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 2^(n-1) mod 20
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the closed‑form expression 2^(n-1) mod 20.
   For n = 1 the value is the special term 1.  For n ≥ 2 we compute
   e = n-1 and use the 4‑cycle of 2^k modulo 20:
          k mod 4 == 0 => 16
          k mod 4 == 1 => 12   (except for k = 1 which gives 2)
          k mod 4 == 2 => 4
          k mod 4 == 3 => 8   */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    /* n >= 2 */
    idx_t e = n - 1;          /* exponent of 2 */
    if (e == 1)              /* 2^1 mod 20 == 2 */
        return (seq_t)2;

    switch (e % 4)
    {
        case 0: return (seq_t)16;  /* 2^4,8,12,... => 16 */
        case 1: return (seq_t)12;  /* 2^5,9,...   => 12 */
        case 2: return (seq_t)4;   /* 2^2,6,10,.. => 4 */
        case 3: return (seq_t)8;   /* 2^3,7,11,.. => 8 */
    }
    /* unreachable, but required to satisfy the compiler */
    return (seq_t)0;
}
