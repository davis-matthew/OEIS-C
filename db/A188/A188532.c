/*
 * A188532  Numbers which contain only the digit 5 in their base-6
 *          representation, with at most one exception. If the exception is
 *          the most-significant digit, it must be the digit 1, 2, 3, or 4,
 *          otherwise the exception must be the digit 4.
 * https://oeis.org/A188532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth smallest integer whose base‑6 representation *
 *            consists solely of the digit 5, with at most one exception. * If
 *            the exception occurs in the most significant position it must be
 *            * a digit 1,2,3,4; otherwise if the exception is elsewhere it
 *            must be the * digit 4
 * Range:     u8 n <= 18, u16 n <= 45, u32 n <= 126, u64 n <= 398,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of the sequence.  The argument n is 1‑based. */
seq_t a(idx_t n)
{
    /* Find the length L of the n-th number. */
    idx_t L = 1;
    while (1) {
        /* Count of admissible numbers of length L:  (L+4). */
        idx_t count = (L + 4);
        if (n <= count)
            break;
        n -= count;
        ++L;
    }

    /* Now n is the rank within all numbers of length L (1‑based). */
    seq_t val = 0;

    if (n <= 4) {
        /* Leading exception digit (1..4) and all following digits are 5. */
        seq_t d = (seq_t)n;
        val = d;
        for (idx_t i = 1; i < L; ++i)
            val = val * (seq_t)6 + (seq_t)5;
    } else {
        /* Leading digit is 5.  Exception may be in a later position or none. */
        val = (seq_t)5;
        idx_t rprime = n - 4;            /* 1‑based index within the leading‑5 group */
        if (rprime <= L - 1) {
            /* Exception at position excPos (1‑based, 2 <= excPos <= L). */
            idx_t excPos = 1 + rprime;   /* excPos ranges 2..L */
            for (idx_t i = 1; i < L; ++i) {
                if (i + 1 == excPos)
                    val = val * (seq_t)6 + (seq_t)4;
                else
                    val = val * (seq_t)6 + (seq_t)5;
            }
        } else {
            /* No exception: all remaining digits are 5. */
            for (idx_t i = 1; i < L; ++i)
                val = val * (seq_t)6 + (seq_t)5;
        }
    }
    return val;
}
