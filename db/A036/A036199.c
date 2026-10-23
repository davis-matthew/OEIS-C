/*
 * A036199  Log base 5 (n) mod 193.
 * https://oeis.org/A036199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = discrete logarithm base 5 of n modulo 193. Find the least
 *            non‑negative k (0 ≤ k ≤ 191) such that 5^k ≡ n (mod 193). If n ≡
 *            0 (mod 193) the value is defined as 0
 * Range:     u8 n <= 192, u16 n <= 192, u32 n <= 192, u64 n <= 192,
 *            u128 n <= 192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Build a small static table mapping every residue 1…192 to its
       discrete log base 5.  The table is filled once on first call.      */
    static seq_t log_table[193];
    static int ready = 0;

    if (!ready) {
        seq_t pow = 1;                  /* 5^0 ≡ 1 (mod 193)            */
        for (idx_t i = 0; i < 192; ++i) {
            log_table[(idx_t)pow] = (seq_t)i;
            pow = (pow * 5) % (seq_t)193; /* advance to next residue      */
        }
        ready = 1;
    }

    idx_t r = n % 193;        /* residue of n modulo 193          */
    if (r == 0)               /* n divisible by 193 → no log      */
        return 0;

    return log_table[r];
}
