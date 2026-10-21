/*
 * A217516  Base-n state complexity of partitioned deterministic finite
 *          automaton (PDFA) for the periodic sequence (1234)*.
 * https://oeis.org/A217516
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A217516: Base‑n state complexity of a PDFA for the periodic sequence (1234)*.
 *  The sequence is ultimately periodic of period 4 with offset 2:
 *      a(2) = 7
 *  For n > 2:
 *      a(n) = 5 if n ≡ 0 (mod 4)
 *      a(n) = 4 if n ≡ 1 (mod 4)
 *      a(n) = 9 if n ≡ 2 (mod 4)
 *      a(n) = 8 if n ≡ 3 (mod 4)
 */

seq_t a(idx_t n)
{
    if (n < 2)                  /* sequence undefined below offset; return 0 */
        return (seq_t)0;
    if (n == 2)
        return (seq_t)7;

    switch (n % 4)
    {
        case 0: return (seq_t)5;   /* n ≡ 0 (mod 4) */
        case 1: return (seq_t)4;   /* n ≡ 1 (mod 4) */
        case 2: return (seq_t)9;   /* n ≡ 2 (mod 4) */
        case 3: return (seq_t)8;   /* n ≡ 3 (mod 4) */
    }
    return (seq_t)0;               /* unreachable, keeps compiler happy */
}
