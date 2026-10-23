/*
 * A019461  Add 1, multiply by 1, add 2, multiply by 2, etc.; start with 0.
 * https://oeis.org/A019461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 25, u64 n <= 41, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; for n≥1 let k=(n+1)/2.
   If n is odd: a(n)=a(n-1)+k,
   If n is even: a(n)=a(n-1)*k. */
seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t k = (i + 1) / 2;            /* operation number */
        if (i & 1) {                     /* odd index – add */
            val += (seq_t)k;
        } else {                         /* even index – multiply */
            val *= (seq_t)k;
        }
    }
    return val;
}
