/*
 * A119518  Abs(*+-) n Sequence.
 * https://oeis.org/A119518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(3)=5, a(4)=1, a(5)=15. For n ≥ 6: if n mod 3
 *            == 0 : a(n) = a(n‑1) – n if n mod 3 == 1 : a(n) = a(n‑1) + n if
 *            n mod 3 == 2 : a(n) = a(n‑1) + (n‑1)² The sequence is the
 *            absolute value of the alternating sum
 *            1+2*3-4+5*6-7+⋯+(n‑2)*(n‑1)-n. The recurrence above reproduces
 *            that sequen...
 * Range:     u8 n <= 13, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety, unused for the sequence */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)1;
    if (n == 5) return (seq_t)15;

    seq_t val = (seq_t)15;                 /* a(5) */
    for (idx_t i = 6; i <= n; ++i)
    {
        if (i % 3 == 0)
        {
            val -= (seq_t)i;               /* a(3k)   = a(3k‑1) – 3k */
        }
        else if (i % 3 == 1)
        {
            val += (seq_t)i;               /* a(3k+1) = a(3k)   + 3k+1 (for k≥1 onwards – handled by base cases) */
        }
        else /* i % 3 == 2 */
        {
            seq_t t = (seq_t)(i - 1);     /* (3k+1) = i‑1 */
            val += t * t;                 /* a(3k+2) = a(3k+1) + (3k+1)²  */
        }
    }
    return val;
}
