/*
 * A179075  Concatenation of the first n numbers in base n.
 * https://oeis.org/A179075
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = n + sum(i * n^(n+1-i), i = 1..n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The computation is performed iteratively by concatenating the base‑n
   representations of 1,2,…,n.  All numbers except the last have one
   digit in base n, and the last number n has two digits ('10'). */
seq_t a(idx_t n)
{
    /* For values below the offset (n < 2) the sequence is not defined
       in OEIS: return 0 to avoid undefined behaviour. */
    if (n < 2)
        return (seq_t)0;

    seq_t result = 0;
    seq_t sn = (seq_t)n;              /* cast n to seq_t once */

    /* Append the numbers 1 through n‑1 (each a single digit). */
    for (idx_t i = 1; i < n; ++i)
        result = result * sn + (seq_t)i;

    /* Append the last number n, which in base n is '10' (two digits). */
    result = result * sn * sn + sn;

    return result;
}
