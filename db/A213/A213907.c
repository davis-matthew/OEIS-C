/*
 * A213907  a(n) = 1 + n + n*{n/2} + n*{n/2}*{n/3} + n*{n/2}*{n/3}*{n/4} +...
 *          where {x} = [x+1/2] = round(x).
 * https://oeis.org/A213907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + n + n*round(n/2) + n*round(n/2)*round(n/3) +
 *            n*round(n/2)*round(n/3)*round(n/4) +
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* round(x) = floor(x + 1/2) is implemented by integer arithmetic as floor((n + k/2) / k). */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t nn = n;
    /* Start sum with the 1 + n part */
    seq_t sum = 1 + nn;
    /* product of round(n/2) * round(n/3) * ... up to current k */
    seq_t term = 1;

    /* iterate k from 2 up to 2*n (round becomes 0 afterwards) */
    idx_t limit = 2 * n;          /* safe for n up to typical bounds */
    for (idx_t k = 2; k <= limit; ++k) {
        /* compute round(n/k) = floor((n + k/2) / k) */
        seq_t r = (nn + (seq_t)(k / 2)) / k;
        /* multiply the running product */
        term *= r;
        if (term == 0)
            break;                /* further terms are zero */
        /* add the current contribution n * term */
        sum += nn * term;
    }
    return sum;
}
