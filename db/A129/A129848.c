/*
 * A129848  Numbers n such that n and 4n share at least one digit.
 * https://oeis.org/A129848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the nth integer n for which n and 4*n share at least one
   decimal digit. The algorithm performs a simple sequential search,
   checking each candidate by building a digit mask for n and for 4*n. */
seq_t a(idx_t n)
{
    idx_t count = 0;                 /* how many qualifying numbers found so far */

    /* Search for the n-th qualifying number. */
    for (idx_t i = 1;; ++i) {
        seq_t num   = (seq_t)i;
        seq_t four  = num << 2;      /* 4 * num */

        uint16_t mask1 = 0, mask2 = 0;

        /* Build mask of digits for num. */
        seq_t t = num;
        while (t > 0) {
            mask1 |= 1u << (unsigned)(t % 10);
            t /= 10;
        }

        /* Build mask of digits for 4*num. */
        t = four;
        while (t > 0) {
            mask2 |= 1u << (unsigned)(t % 10);
            t /= 10;
        }

        /* If the two masks intersect, num qualifies. */
        if (mask1 & mask2) {
            ++count;
            if (count == n)
                return num;          /* found the n-th term */
        }
    }

    /* The function will always return inside the loop; this return is
       only to keep the compiler happy. */
    return 0;
}
