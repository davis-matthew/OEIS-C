/*
 * A189215  Zero-one sequence based on the sequence (3n): a(A008585(k))=a(k);
 *          a(A001651(k))=1-a(k), a(1)=0, a(2)=1.
 * https://oeis.org/A189215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 0 For n > 2: if n % 3 == 0 a(n) = a(n/3) else
 *            a(n) = 1 - a(n - n/3)
 * Range:     u8 n <= 135, u16 n <= 135, u32 n <= 135, u64 n <= 135,
 *            u128 n <= 135, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The rule uses the fact that A008585(k)=3k and
   A001651(k)=k-th positive integer not divisible by 3. */

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)(n == 1 ? 1 : 0);   /* a(1)=1, a(2)=0 */

    /*  Multiples of 3: use the 3‑fold recursion. */
    if ((n % 3) == 0)
        return a(n / 3);

    /*  Non‑multiples of 3: use rank n - floor(n/3). */
    idx_t k = n - (n / 3);   /* number of integers ≤ n that are not multiples of 3 */
    seq_t v = a(k);
    return (seq_t)1 - v;
}
