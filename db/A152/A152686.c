/*
 * A152686  Partial products of the partial products of the nonzero Fibonacci
 *          numbers.
 * https://oeis.org/A152686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} F(k)^(n-k+1) where F(k) is the k‑th Fibonacci
 *            number with F(1)=1, F(2)=1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the value directly in O(n)
   using only integer arithmetic on seq_t and idx_t.  */
seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    seq_t result = (seq_t)1;          /* current value of a(k) */
    seq_t prod_fibs = (seq_t)1;       /* A003266(k) = product_{i=1..k} F(i) */

    /* Fibonacci recurrence: f_prev = F(k-1), f_curr = F(k) */
    seq_t f_prev = (seq_t)0;          /* F(0) */
    seq_t f_curr = (seq_t)1;          /* F(1) */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* fibk = F(k) */
        seq_t fibk = f_curr;

        /* Update partial product of Fibonacci numbers */
        prod_fibs = prod_fibs * fibk;

        /* Update a(k) = a(k-1) * prod_fibs(k) */
        result = result * prod_fibs;

        /* Advance Fibonacci numbers for next iteration */
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }

    return result;
}
