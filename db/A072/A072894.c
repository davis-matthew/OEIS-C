/*
 * A072894  Let c(k) be defined as follows: c(1)=1, c(2)=n, c(k+2) = c(k+1)/2
 *          + c(k)/2 if c(k+1) and c(k) have the same parity; c(k+2) =
 *          c(k+1)/2 + c(k)/2 + 1/2 otherwise; a(n) = limit_{ k -> infinity}
 *          c(k).
 * https://oeis.org/A072894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = limit_{k→∞} c(k) where * c(1)=1, c(2)=n, and *
 *            c(k+2)=ceil((c(k+1)+c(k))/2). * The function returns this limit
 *            for any n ≥ 1
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prev = (seq_t)1;
    seq_t curr = (seq_t)n;

    while (prev != curr)
    {
        /* Compute next = ceil((prev + curr)/2) without overflow.
           Use the identity:
               ceil((x+y)/2) = (x>>1) + (y>>1) + (((x&1)+(y&1)+1)>>1)  */
        seq_t next = (prev >> 1) + (curr >> 1) + (((prev & 1) + (curr & 1) + 1) >> 1);

        prev = curr;
        curr = next;
    }
    return curr;
}
