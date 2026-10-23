/*
 * A158570  a(n) = A007814((2n-1)!! + 1).
 * https://oeis.org/A158570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A007814((2n-1)!! + 1); periodic with period 8: a(n) = 1
 *            if n ≡ 0,1,4,5 (mod 8), 2 if n ≡ 2,6 (mod 8), 4 if n ≡ 3 (mod
 *            8), 5 if n ≡ 7 (mod 8)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 0 is not part of the defined sequence but return 1 for safety */
    if (n == 0)
        return (seq_t)1;

    switch (n & 7U) {          /* n modulo 8 */
        case 0: case 1: case 4: case 5:
            return (seq_t)1;
        case 2: case 6:
            return (seq_t)2;
        case 3:
            return (seq_t)4;
        case 7:
            return (seq_t)5;
    }
    /* unreachable, but required for strict compliance */
    return (seq_t)0;
}
