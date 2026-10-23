/*
 * A034133  a(n) = ceiling((n + 7/10)^3).
 * https://oeis.org/A034133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ceil((n + 7/10)^3)
 * Range:     u8 none, u16 n <= 3, u32 n <= 557, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For smaller n we return 0. */
    if (n == 0) return (seq_t)0;

    /* Convert the index to the sequence integer type first. */
    seq_t n_seq = (seq_t)n;

    /* Compute (10n + 7)^3. */
    seq_t t   = n_seq * (seq_t)10 + (seq_t)7;
    seq_t t2  = t * t;
    seq_t t3  = t2 * t;

    /* ceil(t3 / 1000) = (t3 + 999) / 1000 for positive integers. */
    seq_t result = (t3 + (seq_t)999) / (seq_t)1000;

    return result;
}
