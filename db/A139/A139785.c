/*
 * A139785  First differences of A139784.
 * https://oeis.org/A139785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = a(1) = a(2) = 0.
   For k ≥ 1 let e₁ = 1 and e_{k+1} = 4·e_k – 1.
   For n ≥ 3:
     if n ≡ 3 (mod 4)   a(n) = 2·a(n–1) + e_{(n+1)/4};
     if n ≡ 0 (mod 4)   a(n) = 2·a(n–1) – e_{n/4};
     otherwise          a(n) = 2·a(n–1);
*/
seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;

    /* a[0], a[1], a[2] are 0; start with a[2] as previous value      */
    seq_t prev = (seq_t)0;   /* will hold a[i-1] in the loop          */
    seq_t e   = (seq_t)1;    /* e₁                                 */
    idx_t cur = (idx_t)3;    /* current index to compute             */

    while (cur <= n)
    {
        /* n = 4k – 1   (error + e) */
        prev = prev * (seq_t)2 + e;
        if (cur == n)
            return prev;
        cur++;

        /* n = 4k       (error – e) */
        prev = prev * (seq_t)2 - e;
        if (cur == n)
            return prev;
        cur++;

        /* n = 4k + 1   (no error) */
        prev = prev * (seq_t)2;
        if (cur == n)
            return prev;
        cur++;

        /* n = 4k + 2   (no error) */
        prev = prev * (seq_t)2;
        if (cur == n)
            return prev;
        cur++;

        /* update e to the next block: e_{k+1} = 4·e_k – 1 */
        e = e * (seq_t)4 - (seq_t)1;
    }
    /* should never reach here */
    return prev;
}
