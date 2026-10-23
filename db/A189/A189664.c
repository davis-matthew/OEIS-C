/*
 * A189664  Fixed point of the morphism 0->010, 1->001.
 * https://oeis.org/A189664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2187, u16 n <= 2187, u32 n <= 2187, u64 n <= 2187,
 *            u128 n <= 2187, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(3k-2)=0, a(3k-1)=1-a(k), a(3k)=a(k) for k≥1 */
/* a(0) is defined as 0 for completeness.               */

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, but we handle n==0 safely. */
    if (n == 0) return 0;

    seq_t invert = 0;              /* 0 or 1, accumulates the required flips   */
    while (n % 3 != 1) {           /* Stop when n is of the form 3k-2 (base case) */
        if (n % 3 == 0) {          /* a(3k)   -> a(k)     */
            n = n / 3;
        } else {                   /* n % 3 == 2  -> a(3k-1)=1-a(k)  */
            invert ^= 1;           /* toggle the inversion flag             */
            n = (n + 1) / 3;       /* move to k = (n+1)/3                    */
        }
    }
    /* At this point n is of the form 3k-2 and a(n)=0; apply accumulated inversion. */
    return invert;
}
