/*
 * A075482  Number of iteration that first becomes smaller than the initial
 *          value if Collatz-function (A006370) is iterated, starting with
 *          numbers of the form 64n + 59.
 * https://oeis.org/A075482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the position (1‑based) of the first element in the
 *            Collatz sequence starting at 64·n+59 that is smaller than that
 *            starting value. For 59 the sequence is {59,178,89,268,…,38,…}
 *            and the first below 59 is the 12th term, so a(0)=12
 * Range:     u8 none, u16 n <= 50, u32 n <= 9308, u64 n <= 9308,
 *            u128 n <= 9308, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t start = (seq_t)n * 64 + 59;
    seq_t current = start;
    idx_t pos = 1;                /* first term is the start itself */

    while (current >= start) {
        if (current & 1) {        /* odd */
            current = 3 * current + 1;
        } else {                  /* even */
            current >>= 1;
        }
        pos++;                    /* count the new term */
    }

    return (seq_t)pos;
}
