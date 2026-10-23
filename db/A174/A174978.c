/*
 * A174978  For definition see comments lines.
 * https://oeis.org/A174978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, uned
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A174978
   a(0) = 1;
   For n > 0,
      if n is even:
         let k = n / 2;
         a(n) = 1  if (k + 1) is a power of 2,
                2  otherwise.
      if n is odd:
         let k = (n – 1) / 2;
         a(n) = 5 * a(k) – d,
               where d = 3  if a(k) == 1,
                        5  otherwise. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if ((n & (idx_t)1) == 0) {          /* even case */
        idx_t k = n >> 1;
        /* (k + 1) is a power of two iff (k + 1) & k == 0 */
        return (((k + (idx_t)1) & k) == 0) ? (seq_t)1 : (seq_t)2;
    } else {                            /* odd case */
        idx_t k = (n - (idx_t)1) >> 1;
        seq_t val = a(k);
        seq_t sub = (val == (seq_t)1) ? (seq_t)3 : (seq_t)5;
        return (seq_t)5 * val - sub;
    }
}
