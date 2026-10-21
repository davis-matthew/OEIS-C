/*
 * A097735  Pell equation solutions (8*a(n))^2 - 65*b(n)^2 = -1 with
 *          b(n):=A097736(n), n >= 0.
 * https://oeis.org/A097735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 258*a(n-1) - a(n-2), a(0) = 1, a(1) = 259
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This recurrence generates the sequence A097735 (Pell equation solutions). */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)259;
    seq_t prev2 = (seq_t)1;    /* a(0) */
    seq_t prev  = (seq_t)259;  /* a(1) */
    seq_t cur, i;

    for (i = 2; i <= n; ++i) {
        cur = (seq_t)258 * prev; /* 258*a(n-1) */
        cur -= prev2;            /* -a(n-2) */
        prev2 = prev;
        prev  = cur;
    }
    return prev;
}
