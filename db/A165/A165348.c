/*
 * A165348  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A165348: number of reduced words of length n in a Coxeter group
    with 20 generators S_i and relations (S_i)^2 = (S_i S_j)^9 = I.
    The generating function is
        (t^9 + 2*t^8 + 2*t^7 + 2*t^6 + 2*t^5 + 2*t^4 + 2*t^3 + 2*t^2 + 2*t + 1)
    / (171*t^9 - 18*t^8 - 18*t^7 - 18*t^6 - 18*t^5 - 18*t^4 - 18*t^3 - 18*t^2 - 18*t + 1).
    From this rational G.F. we obtain the linear recurrence
        a(n) = 18*(a(n-1)+a(n-2)+...+a(n-8)) - 171*a(n-9),   for n ≥ 10
    with the initial values
        a(0)=1 , a(1)=20 , a(2)=380 , a(3)=7220 , a(4)=137180 ,
        a(5)=2606420 , a(6)=49521980 , a(7)=940917620 ,
        a(8)=17877434780 , a(9)=339671260630.
*/

#include <stddef.h>  /* for size_t if needed */

seq_t a(idx_t n)
{
    /* pre–computed first ten terms – they fit in uint64_t and are exact
       for any seq_t implementation (built‑in integer or big integer). */
    static const seq_t init[10] = {
        (seq_t)1ULL,
        (seq_t)20ULL,
        (seq_t)380ULL,
        (seq_t)7220ULL,
        (seq_t)137180ULL,
        (seq_t)2606420ULL,
        (seq_t)49521980ULL,
        (seq_t)940917620ULL,
        (seq_t)17877434780ULL,
        (seq_t)339671260630ULL
    };

    if (n < 10) {
        return init[n];
    }

    /* Sliding window holding a[n-9] … a[n-1] during the iteration.
       Only nine elements are needed because the recurrence
       requires a[n-9] and the eight preceding terms. */
    seq_t buf[9];
    for (idx_t i = 0; i < 9; ++i) {
        buf[i] = init[i + 1];          /* buf[0]=a1, … , buf[8]=a9   */
    }

    /* sum of the eight largest terms: a[n-1] … a[n-8]  */
    seq_t sum = 0;
    for (idx_t i = 1; i < 9; ++i) {
        sum += buf[i];
    }

    seq_t new_val;
    for (idx_t k = 10; k <= n; ++k) {
        /* a(k) = 18*sum - 171*buf[0]   (buf[0] holds a(k-9))       */
        new_val = (seq_t)18 * sum - (seq_t)171 * buf[0];

        /* Update sum for the next iteration:
           new_sum = sum - old_second + new_val
           where old_second is buf[1] before the shift.         */
        seq_t old_second = buf[1];
        sum = sum - old_second + new_val;

        /* Shift buffer left by one and store the new term at the end. */
        for (idx_t i = 0; i < 8; ++i) {
            buf[i] = buf[i + 1];
        }
        buf[8] = new_val;
    }

    return buf[8];   /* after the loop, buf[8] holds a(n)              */
}
