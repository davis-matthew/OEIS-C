/*
 * A073300  If n=pqr...st in ternary, a(n)=value of the continuant
 *          [p,q,r,...,s,t].
 * https://oeis.org/A073300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) is the continuant obtained from the base‑3 digits of n. For
 *            n = 0 the continuant is defined as 1
 * Range:     u8 n <= 1942, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the continuant of the base‑3 expansion of n. */
seq_t a(idx_t n)
{
    /* The value 1 is the continuant of the empty digit list,
       which we need for n = 0. */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    /* Extract the base‑3 digits (least significant first). */
    uint8_t digits[64];
    int len = 0;
    idx_t t = n;
    while (t > (idx_t)0) {
        digits[len++] = (uint8_t)(t % (idx_t)3);
        t /= (idx_t)3;
    }

    /* Compute the continuant iteratively.
       K([]) = 1, K([x]) = x, and
       K(a1,…,an) = a1*K(a2,…,an) + K(a3,…,an). */
    seq_t prev_prev = (seq_t)0;  /* K of empty suffix two steps back   */
    seq_t prev      = (seq_t)1;  /* K of empty suffix one step back    */
    for (int i = len - 1; i >= 0; --i) {
        seq_t x   = (seq_t)digits[i];
        seq_t cur = x * prev + prev_prev;
        prev_prev = prev;
        prev = cur;
    }
    return prev;
}
