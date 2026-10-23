/*
 * A193680  Period 6 sequence 0,1,2,0,2,1.
 * https://oeis.org/A193680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n (mod 3) if (-1)^floor(n/3)=+1 else (3 - n) mod 3
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n % 6)
    {
        case 0: return 0;
        case 1: return 1;
        case 2: return 2;
        case 3: return 0;
        case 4: return 2;
        case 5: return 1;
        default: return 0; /* unreachable */
    }
}
