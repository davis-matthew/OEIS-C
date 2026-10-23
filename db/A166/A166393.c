/*
 * A166393  Multiples of 23 whose digit reversal + 1 is also a multiple of 23.
 * https://oeis.org/A166393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 15, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search for multiples of 23 whose decimal
 * reversal plus 1 is also a multiple of 23.  The algorithm
 * iterates through successive multiples of 23 and tests the condition
 * until the nth term is found.  It uses only integer arithmetic. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for offset 1, but safe return */

    const seq_t MOD = (seq_t)23;
    seq_t count = 0;
    idx_t k = 1;           /* multiplier for 23 */

    for (;; ++k)
    {
        seq_t m = (seq_t)23 * (seq_t)k;

        /* compute decimal reversal of m */
        seq_t rev = 0;
        seq_t tmp = m;
        while (tmp > 0)
        {
            rev = rev * 10 + (tmp % 10);
            tmp /= 10;
        }

        /* check whether reversed number plus one is divisible by 23 */
        if ((rev + 1) % MOD == 0)
        {
            ++count;
            if (count == n)
                return m;
        }
    }
}
