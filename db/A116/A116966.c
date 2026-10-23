/*
 * A116966  a(n) = n + {1,2,0,1} according as n == {0,1,2,3} mod 4.
 * https://oeis.org/A116966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + {1,2,0,1} according as n == {0,1,2,3} mod 4
 * Range:     u8 n <= 254, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)n;
    switch ((n & 3)) {
        case 0: result += 1; break;
        case 1: result += 2; break;
        case 2: /* no offset */ break;
        case 3: result += 1; break;
    }
    return result;
}
