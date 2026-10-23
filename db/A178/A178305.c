/*
 * A178305  a(0)=1, a(1)=a(2)=1 and a(3k)=a(k), a(3k+1) = a(3k+2) = 2a(k) for
 *          k >= 1.
 * https://oeis.org/A178305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0; a(1)=a(2)=1; For k ≥ 1: a(3k) = a(k) a(3k+1) = 2·a(k)
 *            a(3k+2) = 2·a(k)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n).  Uses only integer arithmetic on seq_t and idx_t. */
seq_t a(idx_t n)
{
    if (n == 0)            return (seq_t)0;
    if (n == 1 || n == 2)  return (seq_t)1;

    idx_t k   = n / 3;
    seq_t val = a(k);

    /* Multiply by 2 if the least‑significant ternary digit is 1 or 2. */
    return (n % 3 == 0) ? val : (val << 1);
}
