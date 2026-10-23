/*
 * A035485  Card on top of deck at n-th stage of R. K. Guy's shuffling
 *          problem.
 * https://oeis.org/A035485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = result of applying the inverse shuffle mapping from k = n
 *            down to 1, starting with the top card (position 1)
 * Range:     u8 n <= 142, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cur = 1;                     /* position of the top card after all shuffles */
    for (idx_t k = n; k != 0; --k) {
        seq_t two_k = (seq_t)k + (seq_t)k;   /* 2 * k, but computed without overflow risk for small k */
        if (cur > two_k) {
            /* card lies beyond the first 2k positions; its position stays the same */
        } else if (cur & 1) {                  /* cur is odd → came from the second pile */
            cur = (seq_t)k + ((cur + 1) >> 1);
        } else {                              /* cur is even → came from the first pile */
            cur = cur >> 1;
        }
    }
    return cur;
}
