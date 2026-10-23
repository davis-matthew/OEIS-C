/*
 * A044927  a(n)=so-se, where so(se)=sum of odd(even) base 5 run lengths of n.
 * https://oeis.org/A044927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) = (sum of odd-length base‑5 run lengths) –
                    (sum of even-length base‑5 run lengths) of n. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1; below offset is irrelevant */

    idx_t m = n;                          /* working copy of n */
    unsigned prev = m % 5;                /* first base‑5 digit (least significant) */
    m /= 5;
    seq_t len = 1;                        /* current run length */
    seq_t sum_odd = 0;
    seq_t sum_even = 0;

    while (m != 0) {
        unsigned d = m % 5;
        m /= 5;
        if (d == prev) {
            len += 1;
        } else {
            if (len % 2 == 1) sum_odd += len;
            else             sum_even += len;
            prev = d;
            len = 1;
        }
    }

    /* finish last run */
    if (len % 2 == 1) sum_odd += len;
    else             sum_even += len;

    return (seq_t)(sum_odd - sum_even);
}
