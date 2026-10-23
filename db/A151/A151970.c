/*
 * A151970  a(n) = smallest integer >= n which has only prime factors 3 and 5.
 * https://oeis.org/A151970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer >= n that has only the prime factors 3
 *            and 5
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t best = 0;          /* best candidate found so far            */
    seq_t pow3 = 1;          /* current value of 3^i                    */
    seq_t threshold = (seq_t)n;

    for (;;)
    {
        seq_t cand;
        /* Find the smallest multiple of pow3 by powers of 5 that reaches threshold */
        if (pow3 >= threshold)
            cand = pow3;
        else
        {
            seq_t t = pow3;
            while (t < threshold)
                t = t * 5;
            cand = t;
        }

        /* Update best candidate */
        if (best == 0 || cand < best)
            best = cand;

        /* All larger powers of 3 will produce candidates >= pow3, thus >= best,
           so we can stop if we already found a candidate smaller than pow3. */
        if (best != 0 && pow3 > best)
            break;

        /* Prepare next power of 3 for the next iteration */
        pow3 = pow3 * 3;
    }

    return best;
}
