/*
 * A100404  a(n) = L(n) * n! where L(n) are the Lucas numbers.
 * https://oeis.org/A100404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = L(n) * n! ; where Lucas numbers L satisfy L(0) = 2, L(1)
 *            = 1, L(n) = L(n-1) + L(n-2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = L(n) * n! for any n >= 0.
   The function works for arbitrary-sized integer types
   provided as seq_t and idx_t by the build system. */
seq_t a(idx_t n)
{
    /* special cases for n = 0 and n = 1 */
    if (n == 0) return (seq_t)2;   /* L(0)*0! = 2*1 */
    if (n == 1) return (seq_t)1;   /* L(1)*1! = 1*1 */

    /* initialise Lucas numbers and factorial */
    seq_t l_prev = (seq_t)2;   /* L(0) */
    seq_t l_curr = (seq_t)1;   /* L(1) */
    seq_t fact   = (seq_t)1;   /* current factorial, start at 0! */

    /* iterate from n = 2 up to the desired n */
    for (idx_t i = 2; i <= n; ++i) {
        /* compute next Lucas number */
        seq_t l_next = l_prev + l_curr;
        l_prev = l_curr;
        l_curr = l_next;

        /* update factorial: fact = i!   (i includes current step) */
        fact = fact * (seq_t)i;
    }

    /* result is L(n) * n! */
    return l_curr * fact;
}
