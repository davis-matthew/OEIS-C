/*
 * A069211  Let u(n,k) be the recursion: u(n,1)=1, u(n,2)=n, u(n,k+2) = (1/2)
 *          * (u(n,k+1)+u(n,k)) if u(n,k+1)+u(n,k) is even, and u(n,k+2) =
 *          abs(u(n,k+1)-u(n,k)) otherwise. Sequence gives integer values a(n)
 *          such that u(n,k)=1 for any k>=a(n).
 * https://oeis.org/A069211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A069211:  a(n) = index of the first of two consecutive 1's in the
 *  sequence defined by u_1 = 1, u_2 = n, u_{k+2} = (u_{k+1}+u_k)/2
 *  if the sum is even, otherwise u_{k+2} = |u_{k+1}-u_k|.
 *
 *  The function returns the minimal k such that u_k = u_{k+1} = 1.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)           /* sequence offset starts at 1; 0 is unused */
        return 0;

    seq_t prevprev = 1;    /* u_1 */
    seq_t prev     = n;    /* u_2 */

    /* handle the trivial case a(1) = 1 */
    if (prevprev == 1 && prev == 1)
        return 1;

    idx_t k = 2;           /* current index of 'prev' */

    for (;;)
    {
        /* compute u_{k+2} without overflowing seq_t */
        unsigned __int128 sum128 = (unsigned __int128)prev + (unsigned __int128)prevprev;
        seq_t next;
        if ((sum128 & 1) == 0)          /* even sum */
            next = (seq_t)(sum128 >> 1); /* (prev + prevprev) / 2 */
        else                            /* odd sum */
            next = (prev >= prevprev) ? (prev - prevprev) : (prevprev - prev);

        ++k;                 /* now 'next' is u_{k+1} */

        /* shift window */
        prevprev = prev;
        prev     = next;

        /* check for two consecutive ones */
        if (prevprev == 1 && prev == 1)
            return k - 1;   /* first of the pair is at index k-1 */
    }
}
