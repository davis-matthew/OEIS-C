/*
 * A120710  A GF(2) polynomial analog of triangular numbers.
 * https://oeis.org/A120710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 639, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A120710
   Recurrence: a(0)=0;
               a(n+2^k) = a(n) XOR (n*2^k)   for 0 ≤ n < 2^k      */
seq_t a(idx_t n)
{
    /* Special case: the first term is 0. */
    if (n == 0u)
        return (seq_t)0;

    seq_t result = 0;
    idx_t cur = n;

    while (cur != 0u) {
        /* Find the highest power of two ≤ cur.  __builtin_clzll is
           defined for unsigned long long, which is at least 64 bits.
           cur is never 0 here, so the call is safe. */
        idx_t pow = (idx_t)1ULL << (63 - __builtin_clzll(cur));

        idx_t next = cur - pow;               /* 0 ≤ next < pow      */
        result ^= ((seq_t)next * (seq_t)pow); /* recurrence step      */
        cur = next;                           /* continue with smaller n */
    }

    return result;
}
