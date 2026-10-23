/*
 * A123170  Continued fraction for (sqrt(2) + 1/sqrt(2))*tanh(1/sqrt(2)).
 * https://oeis.org/A123170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(n) follows a 10-term periodic pattern. For n = 1..10 the
 *            values are pre-specified. For n >= 11 let b = ceil(n/10). Then
 *            letting pos = n - 10*(b-1) (so pos = 1..10), the value is: pos
 *            1: a = 4*b - 4 pos 2: a = 1 pos 3: a = 2 pos 4: a = 8*b - 5 pos
 *            5: a = 36*b - 15 pos 6: a = 8*b - 2 pos 7: a = 3...
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first block explicitly */
    switch (n) {
        case 1:  return 1;
        case 2:  return 3;
        case 3:  return 2;
        case 4:  return 3;
        case 5:  return 21;
        case 6:  return 6;
        case 7:  return 33;
        case 8:  return 8;
        case 9:  return 1;
        case 10: return 2;
        default: break;
    }

    /* Compute block number (b ≥ 2) and position within the block (1..10) */
    idx_t b = (n + 9) / 10;             /* ceil(n/10) */
    idx_t pos = n - 10 * (b - 1);       /* 1-based position in the block */

    seq_t sb = (seq_t)b;                /* cast to seq_t for arithmetic */

    switch (pos) {
        case 1:  return sb * 4 - 4;
        case 2:  return 1;
        case 3:  return 2;
        case 4:  return sb * 8 - 5;
        case 5:  return sb * 36 - 15;
        case 6:  return sb * 8 - 2;
        case 7:  return sb * 36 - 3;
        case 8:  return sb * 8;
        case 9:  return 1;
        case 10: return 2;
        default: return 0; /* unreachable, safety */
    }
}
