/*
 * A050271  Numbers k such that k = floor(sqrt(k)*ceiling(sqrt(k))).
 * https://oeis.org/A050271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n + 4) / 3)^2 + ((n + 1) mod 3) - 2
 * Range:     u8 n <= 45, u16 n <= 765, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes OEIS A050271: numbers k such that k = floor(sqrt(k)*ceil(sqrt(k))). */

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 1. For n == 0 return 0 (undefined). */
    if (n == 0) return 0;

    seq_t n_seq = (seq_t)n;        /* Convert index to seq_t. */
    seq_t m     = (n_seq + 4) / 3; /* floor((n+4)/3). */
    seq_t part  = m * m;           /* m^2. */
    seq_t mod   = (n_seq + 1) % 3; /* (n+1) mod 3. */
    return part + mod - 2;
}
