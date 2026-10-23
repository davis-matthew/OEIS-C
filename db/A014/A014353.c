/*
 * A014353  Chance of getting nothing, a pair, 2 pair, 3 of a kind, other
 *          straight, other flush, full house, 4 of a kind, other straight
 *          flush, or a royal flush in poker is 1 in a(n) (rounded to nearest
 *          integer).
 * https://oeis.org/A014353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = round( C(52,5) / count_of_category(n) )
 * Range:     u8 none, u16 none, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* count_of_category(n) is the number of 5‑card poker hands that
   belong to the n‑th rank (1 = high card, 2 = one pair, …, 10 = royal flush). */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Total number of 5‑card hands */
    const seq_t total = (seq_t)2598960;

    /* Hand counts for the 10 categories, in order of the sequence
       (see the OEIS entry description). */
    static const seq_t counts[10] = {
        (seq_t)1302540, /* high card      */
        (seq_t)1098240, /* one pair       */
        (seq_t)123552,  /* two pair       */
        (seq_t)54912,   /* three of a kind*/
        (seq_t)10200,   /* straight (non flush) */
        (seq_t)5108,    /* flush (non straight) */
        (seq_t)3744,    /* full house     */
        (seq_t)624,     /* four of a kind */
        (seq_t)36,      /* straight flush (non‑royal) */
        (seq_t)4        /* royal flush    */
    };

    if (n < 1 || n > 10)
        return (seq_t)0;   /* terms beyond the defined range are undefined */

    seq_t cnt = counts[n - 1];
    seq_t half = cnt / 2;      /* floor(cnt/2)   */
    seq_t num  = total + half; /* numerator for rounding */
    return num / cnt;          /* floor((total + half)/cnt)  */
}
