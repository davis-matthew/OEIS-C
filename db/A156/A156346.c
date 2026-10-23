/*
 * A156346  Period length 12: repeat 1,2,-4,4,-2,-1,-1,-2,4,-4,2,1.
 * https://oeis.org/A156346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 71, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A156346: a(0)=1, a(1)=2, a(2)=-4, a(3)=4, a(4)=-2, a(5)=-1,
   a(n+6) = -a(n)  (period 12) */
seq_t a(idx_t n)
{
    /* Base values for n = 0..5.  They are computed once, no lookup
       beyond these six constants. */
    static const seq_t base[6] = {1, 2, -4, 4, -2, -1};

    /* Reduce n modulo 6 to find which base value applies. */
    idx_t r = n % 6;
    seq_t val = base[r];

    /* Flip sign every block of six terms. */
    if ((n / 6) & 1) {
        val = -val;
    }

    return val;
}
