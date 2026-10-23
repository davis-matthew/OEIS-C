/*
 * A178463  The number of iterations for the map : r -> !d(1)+ !d(2)+...+
 *          !d(r) starting with n, needed to reach 0 or stabilize, where d(i),
 *          i=1..r, are the decimal digits of n.
 * https://oeis.org/A178463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of iterations of r -> Σ !d(i) starting with n,
 *            needed to reach 0 or a fixed point, where d(i) are the decimal
 *            digits of r and !k is the subfactorial (derangement) of the
 *            digit k
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Subfactorial values for digits 0-9. */
static const seq_t subfact[10] = {
    (seq_t)1,   /* !0 = 1   */
    (seq_t)0,   /* !1 = 0   */
    (seq_t)1,   /* !2 = 1   */
    (seq_t)2,   /* !3 = 2   */
    (seq_t)9,   /* !4 = 9   */
    (seq_t)44,  /* !5 = 44  */
    (seq_t)265, /* !6 = 265 */
    (seq_t)1854,/* !7 = 1854*/
    (seq_t)14833,/* !8 = 14833*/
    (seq_t)133496/* !9 = 133496*/
};

/* Compute the sum of subfactorials of the decimal digits of x. */
static seq_t digit_subfactorial_sum(seq_t x)
{
    if (x == (seq_t)0) return subfact[0];
    seq_t sum = (seq_t)0;
    while (x > (seq_t)0) {
        unsigned d = (unsigned)(x % (seq_t)10); /* digit 0‑9 fits in unsigned */
        sum += subfact[d];
        x /= (seq_t)10;
    }
    return sum;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Special handling for n == 0: by definition a(0) = 2. */
    if (n == (idx_t)0) return (seq_t)2;

    seq_t current = (seq_t)n;
    seq_t iter    = (seq_t)0;

    while (true) {
        seq_t next = digit_subfactorial_sum(current);

        /* If the next value equals the current one, we are at a fixed point. */
        if (next == current) return iter;

        /* If the next value is zero, we stop after this iteration. */
        if (next == (seq_t)0) return iter + (seq_t)1;

        /* Prepare for the next iteration. */
        iter++;
        current = next;
    }
}
