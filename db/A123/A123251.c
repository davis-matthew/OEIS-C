/*
 * A123251  Continued fraction for sqrt(2)*tan(1/sqrt(2)).
 * https://oeis.org/A123251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   for k>=1,
 *            a(12*k-11)=a(12*k-9)=a(12*k-7)=a(12*k-5)=a(12*k-3)=a(12*k-1)=1;
 *            a(12*k-10)=24*k-20; a(12*k-8)=12*k-9; a(12*k-6)=24*k-12;
 *            a(12*k-4)=12*k-5; a(12*k-2)=24*k-4; a(12*k)=12*k-1
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t r = n % 12;
    switch (r)
    {
    case 1: case 3: case 5: case 7: case 9: case 11:
        return 1;
    case 0:
        { idx_t g = n / 12; return (seq_t)12 * g - 1; }
    case 2:
        { idx_t g = (n + 10) / 12; return (seq_t)24 * g - 20; }
    case 4:
        { idx_t g = (n + 8) / 12; return (seq_t)12 * g - 9; }
    case 6:
        { idx_t g = (n + 6) / 12; return (seq_t)24 * g - 12; }
    case 8:
        { idx_t g = (n + 4) / 12; return (seq_t)12 * g - 5; }
    case 10:
        { idx_t g = (n + 2) / 12; return (seq_t)24 * g - 4; }
    default:
        return 0;
    }
}
