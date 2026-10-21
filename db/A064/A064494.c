/*
 * A064494  Shotgun (or Schrotschuss) numbers: limit of the recursion B(k)
 *          =T[k](B(k-1)), where B(1) = (1,2,3,4,5,...) and T[k] is the
 *          Transformation that permutes the entries k(2i-1) and k(2i) for all
 *          positive integers i.
 * https://oeis.org/A064494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Shotgun (Schrotschuss) numbers.
    a(n) is obtained by reversing the sequence of swaps T[k] (k = n … 2).
    For a current position p, if p is a multiple of k, let t = p / k.
    If t is odd, the partner is p + k; if t is even, the partner is p – k.
    The integer returned is the original index that ends up at position n. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* no defined term for n = 0 */

    seq_t pos = (seq_t)n;

    for (idx_t k = n; k >= 2; --k)        /* iterate k = n .. 2   */
    {
        seq_t k_seq = (seq_t)k;
        if (pos % k_seq == 0)              /* p is in a pair for this k */
        {
            seq_t t = pos / k_seq;
            if ((t & 1u) == 1)             /* odd t → partner larger   */
                pos = pos + k_seq;
            else                          /* even t → partner smaller */
                pos = pos - k_seq;
        }
    }
    return pos;
}
