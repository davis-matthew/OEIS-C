/*
 * A021212  Decimal expansion of 1/208.
 * https://oeis.org/A021212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = 0,0,4,8 for n = 0,1,2,3, thereafter 0,7,6,9,2,3 repeating
 *            with period 6 (i.e. a(n) = seq[(n-4)%6] for n≥4)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)
    {
        switch ((unsigned)n)
        {
            case 0: return (seq_t)0;
            case 1: return (seq_t)0;
            case 2: return (seq_t)4;
            case 3: return (seq_t)8;
            default: break; /* unreachable */
        }
    }
    idx_t r = (n - 4) % 6;  /* 0..5 */
    switch (r)
    {
        case 0: return (seq_t)0;
        case 1: return (seq_t)7;
        case 2: return (seq_t)6;
        case 3: return (seq_t)9;
        case 4: return (seq_t)2;
        case 5: return (seq_t)3;
        default: return (seq_t)0;  /* unreachable */
    }
}
