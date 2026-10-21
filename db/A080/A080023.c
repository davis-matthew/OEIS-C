/*
 * A080023  log_phi(n) is closer to an integer than is log_phi(m) for any m
 *          with 2<=m<n, where phi=(1+sqrt(5))/2 is the golden ratio.
 * https://oeis.org/A080023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Lucas numbers without the term 1 a(n) = Lucas(n) except that
 *            Lucas(1)=1 is omitted Input: n >= 1 with offset 1 (a(1)=2) The
 *            function iteratively generates Lucas numbers and skips the
 *            single value 1
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)        /* out of range; not used but defined */
        return 0;

    seq_t prev = 2;    /* L_0 */
    seq_t curr = 1;    /* L_1 */
    idx_t  out  = 0;   /* number of values already output */
    seq_t  result = 0;

    while (out < n)
    {
        if (prev != (seq_t)1)          /* skip the lone term 1 */
        {
            ++out;
            if (out == n)
            {
                result = prev;
                break;
            }
        }
        seq_t next = prev + curr;      /* Lucas recurrence L_k = L_{k-1}+L_{k-2} */
        prev = curr;
        curr = next;
    }
    return result;
}
