/*
 * A145127  a(n) = 1 + (144 + (50 + (35 + (10 + n)*n)*n)*n)*n/120.
 * https://oeis.org/A145127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + (144 + (50 + (35 + (10 + n)*n)*n)*n)*n/120
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 218, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 nn = n;
    unsigned __int128 t = nn *
        ( 144u
          + ( 50u
              + ( 35u
                  + ( 10u + nn ) * nn ) * nn ) * nn );
    unsigned __int128 res = 1 + t / 120u;
    return (seq_t)res;
}
