/*
 * A038179  Result of second stage of sieve of Eratosthenes (after eliminating
 *          multiples of 2 and 3).
 * https://oeis.org/A038179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038179: second stage of the sieve of Eratosthenes
   (terms are 2, 3 and all integers of the form 6m ± 1 for m ≥ 1.
   Formula for n ≥ 3:
     let k = n - 3,
     t = floor(k/2) + 1,
     a(n) = 6*t + (k is odd ? +1 : -1).
   Base cases: a(1)=2, a(2)=3. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;

    /* n >= 3 */
    idx_t k = n - 3;                /* 0,1,2,3,...  */
    idx_t t = (k >> 1) + 1;         /* t = floor(k/2)+1 */
    seq_t m = (seq_t)t;             /* conversion to seq_t   */
    seq_t val = 6 * m;              /* 6 * t                 */

    if (k & 1)                      /* odd k -> +1          */
        val += 1;
    else                            /* even k -> -1         */
        val -= 1;

    return val;
}
