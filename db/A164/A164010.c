/*
 * A164010  Zero together with row 10 of the array in A163280.
 * https://oeis.org/A164010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 + 9n for n >= 17; a(0..16) are the specified values
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 16)
    {
        switch (n)
        {
            case 0:  return (seq_t)0;
            case 1:  return (seq_t)23;
            case 2:  return (seq_t)46;
            case 3:  return (seq_t)57;
            case 4:  return (seq_t)92;
            case 5:  return (seq_t)85;
            case 6:  return (seq_t)138;
            case 7:  return (seq_t)119;
            case 8:  return (seq_t)152;
            case 9:  return (seq_t)171;
            case 10: return (seq_t)200;
            case 11: return (seq_t)220;
            case 12: return (seq_t)276;
            case 13: return (seq_t)286;
            case 14: return (seq_t)322;
            case 15: return (seq_t)375;
            case 16: return (seq_t)416;
        }
    }

    seq_t nn = (seq_t)n;          /* cast index to seq_t for arithmetic */
    return nn * nn + ((seq_t)9) * nn;
}
