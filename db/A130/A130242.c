/*
 * A130242  Minimal index k of a Lucas number such that Lucas(k)>=n (the
 *          'upper' Lucas (A000032) Inverse).
 * https://oeis.org/A130242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 227, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the minimal index k such that Lucas(k) ≥ n. */
/* This function simply generates Lucas numbers in ascending order
   until the first value that is ≥ n is found. */

seq_t a(idx_t n)
{
    /* For n = 0,1,2 the smallest Lucas value (2) is ≥ n, so the
       answer is 0. */
    if (n <= (seq_t)2) {
        return (seq_t)0;
    }

    /* Lucas(0) = 2, Lucas(1) = 1.  We keep the last two Lucas values. */
    seq_t Lk   = (seq_t)2;   /* L(k)   */
    seq_t Lk1  = (seq_t)1;   /* L(k+1) */
    idx_t k    = 0;          /* current index k   */

    /* Generate successive Lucas numbers until L(k) ≥ n.  */
    while (Lk < n) {
        idx_t k_next = k + 1;           /* next index  */
        seq_t L_next = Lk + Lk1;        /* L(k+2) = L(k+1) + L(k) */
        Lk  = Lk1;                     /* shift forward */
        Lk1 = L_next;
        k = k_next;
    }

    /* k is the minimal index such that Lucas(k) ≥ n */
    return (seq_t)k;
}
